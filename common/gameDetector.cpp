/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001  Ludvig Strigeus
 * Copyright (C) 2001-2003 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */

#include "stdafx.h"
#include "sound/mididrv.h"
#include "engine.h"
#include "gameDetector.h"
#include "config-file.h"
#include "scaler.h"	// Only for gfx_modes

#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#define CHECK_OPTION() if ((current_option != NULL) || (*s != '\0')) goto ShowHelpAndExit
#define HANDLE_OPTION() if ((*s == '\0') && (current_option == NULL)) goto ShowHelpAndExit;  \
                        if ((*s != '\0') && (current_option != NULL)) goto ShowHelpAndExit; \
                        option = (*s == '\0' ? current_option : s);                         \
                        current_option = NULL
#define HANDLE_OPT_OPTION() if ((*s != '\0') && (current_option != NULL)) goto ShowHelpAndExit; \
                            if ((*s == '\0') && (current_option == NULL)) option = NULL;         \
                            else option = (*s == '\0' ? current_option : s);                    \
                            current_option = NULL

// DONT FIXME: DO NOT ORDER ALPHABETICALY, THIS IS ORDERED BY IMPORTANCE/CATEGORY! :)
#ifdef __PALM_OS__
static const char USAGE_STRING[] = "NoUsageString"; // save more data segment space
#else
static const char USAGE_STRING[] = 
	"ScummVM - Scumm Interpreter\n"
	"Syntax:\n"
	"\tscummvm [-v] [-d[<num>]] [-n] [-b<num>] [-t<num>] [-s<num>] [-p<path>] [-m<num>] [-f] game\n"
	"Flags:\n"
	"\t-p<path>      - look for game in <path>\n"
	"\t-x[<num>]     - load this savegame (default: 0 - autosave)\n"
	"\t-f            - fullscreen mode\n"
	"\t-g<mode>      - graphics mode (normal,2x,3x,2xsai,super2xsai,supereagle,advmame2x,advmame3x,tv2x,dotmatrix)\n"
	"\t-e<mode>      - set music engine (see README for details)\n"
	"\t-a            - specify game is amiga version\n"
	"\t-q<lang>      - specify language (en,de,fr,it,pt,es,jp,zh,kr,hb)\n"
	"\n"
	"\t-c<num>       - use cdrom <num> for cd audio\n"
	"\t-m<num>       - set music volume to <num> (0-255)\n"
	"\t-o<num>       - set master volume to <num> (0-255)\n"
	"\t-s<num>       - set sfx volume to <num> (0-255)\n"
	"\t-t<num>       - set music tempo (50-200, default 100%%)\n"
	"\n"
	"\t-n            - no subtitles for speech\n"
	"\t-y            - set text speed (default: 60)\n"
	"\n"
	"\t-l<file>      - load config file instead of default\n"
#if defined(UNIX)
	"\t-w[<file>]    - write to config file [~/.scummvmrc]\n"
#else
	"\t-w[<file>]    - write to config file [scummvm.ini]\n"
#endif
	"\t-v            - show version info and exit\n"
	"\t-z            - display list of games\n"
	"\n"
	"\t-b<num>       - start in room <num>\n"
	"\t-d[<num>]     - enable debug output (debug level [1])\n"
	"\t-u            - dump scripts\n"
	"\n"
	"\t--multi-midi  - enable combination Adlib and native MIDI\n"
	"\t--native-mt32 - true Roland MT-32 (disable GM emulation)\n"
;
#endif
// This contains a pointer to a list of all supported games.
const VersionSettings *version_settings = NULL;

static const struct GraphicsMode gfx_modes[] = {
	{"normal", "Normal (no scaling)", GFX_NORMAL},
	{"1x", "Normal (no scaling)", GFX_NORMAL},
#ifndef __PALM_OS__	// reduce contant data size
	{"2x", "2x", GFX_DOUBLESIZE},
	{"3x", "3x", GFX_TRIPLESIZE},
	{"2xsai", "2xSAI", GFX_2XSAI},
	{"super2xsai", "Super2xSAI", GFX_SUPER2XSAI},
	{"supereagle", "SuperEagle", GFX_SUPEREAGLE},
	{"advmame2x", "AdvMAME2x", GFX_ADVMAME2X},
	{"advmame3x", "AdvMAME3x", GFX_ADVMAME3X},
	{"tv2x", "TV2x", GFX_TV2X},
	{"dotmatrix", "DotMatrix", GFX_DOTMATRIX},
#else
	{"flipping", "Page Flipping", GFX_FLIPPING},
	{"dbuffer", "Double Buffer", GFX_DOUBLEBUFFER},
	{"wide", "Wide (HiRes+ only)", GFX_WIDE},
#endif
	{0, 0, 0}
};

static const struct Language languages[] = {
	{"en", "English", EN_USA},
	{"de", "German", DE_DEU},
	{"fr", "French", FR_FRA},
	{"it", "Italian", IT_ITA},
	{"pt", "Portuguese", PT_BRA},
	{"es", "Spanish", ES_ESP},
	{"jp", "Japanese", JA_JPN},
	{"zh", "Chinese (Taiwan)", ZH_TWN},
	{"kr", "Korean", KO_KOR},
	{"hb", "Hebrew", HB_HEB},
	{0, 0, 0}
};

static const struct MusicDriver music_drivers[] = {
	{"auto", "Default", MD_AUTO},
	{"null", "No music", MD_NULL},
#ifndef __PALM_OS__	// reduce contant data size
	{"windows", "Windows MIDI", MD_WINDOWS},
	{"seq", "SEQ", MD_SEQ},
	{"qt", "QuickTime", MD_QTMUSIC},
	{"core", "CoreAudio", MD_COREAUDIO},
	{"etude", "Etude", MD_ETUDE},
	{"alsa", "ALSA", MD_ALSA},
	{"adlib", "Adlib", MD_ADLIB},
#else
	{"ypa1", "Yamaha Pa1", MD_YPA1},
#endif
	{0, 0, 0}
};

static int countVersions(const VersionSettings *v) {
	int count;
	for (count = 0; v->filename; v++, count++)
		;
	return count;
}

GameDetector::GameDetector() {
	_fullScreen = false;

	_use_adlib = false;

	_master_volume = kDefaultMasterVolume;
	_music_volume = kDefaultMusicVolume;
	_sfx_volume = kDefaultSFXVolume;
	_amiga = false;
	_language = 0;

	_talkSpeed = 60;
	_debugMode = 0;
	_debugLevel = 0;
	_dumpScripts = 0;
	_noSubtitles = false;
	_bootParam = 0;

	_gameDataPath = 0;
	_gameTempo = 0;
	_midi_driver = MD_AUTO;
	_game.id = 0;
	_game.features = 0;

	_multi_midi = false;
	_native_mt32 = false;

	_cdrom = 0;
	_save_slot = 0;
	
	_saveconfig = false;
	
#ifndef _WIN32_WCE
	_gfx_mode = GFX_DOUBLESIZE;
#else
	_gfx_mode = GFX_NORMAL;
#endif
	_default_gfx_mode = true;

	if (version_settings == NULL) {
		int totalCount = 0;
		
		// Gather & combine the target lists from the modules

#ifndef DISABLE_SCUMM
		const VersionSettings *scummVersions = Engine_SCUMM_targetList();
		int scummCount = countVersions(scummVersions);
		totalCount += scummCount;
#endif

#ifndef DISABLE_SIMON
		const VersionSettings *simonVersions = Engine_SIMON_targetList();
		int simonCount = countVersions(simonVersions);
		totalCount += simonCount;
#endif

#ifndef DISABLE_SKY
		const VersionSettings *skyVersions = Engine_SKY_targetList();
		int skyCount = countVersions(skyVersions);
		totalCount += skyCount;
#endif
		
		VersionSettings *v = (VersionSettings *)calloc(totalCount + 1, sizeof(VersionSettings));
		version_settings = v;

#ifndef DISABLE_SCUMM
		memcpy(v, scummVersions, scummCount * sizeof(VersionSettings));
		v += scummCount;
#endif

#ifndef DISABLE_SIMON
		memcpy(v, simonVersions, simonCount * sizeof(VersionSettings));
		v += simonCount;
#endif

#ifndef DISABLE_SKY
		memcpy(v, skyVersions, skyCount * sizeof(VersionSettings));
		v += skyCount;
#endif
	}
}

void GameDetector::updateconfig() {
	const char *val;

	_amiga = g_config->getBool("amiga", _amiga);

	_save_slot = g_config->getInt("save_slot", _save_slot);

	_cdrom = g_config->getInt("cdrom", _cdrom);

	if ((val = g_config->get("music_driver")))
		if (!parseMusicDriver(val)) {
			printf("Error in the config file: invalid music_driver.\n");
			printf(USAGE_STRING);
			exit(-1);
		}

	_fullScreen = g_config->getBool("fullscreen", _fullScreen);

	if ((val = g_config->get("gfx_mode")))
		if ((_gfx_mode = parseGraphicsMode(val)) == -1) {
			printf("Error in the config file: invalid gfx_mode.\n");
			printf(USAGE_STRING);
			exit(-1);
		}

	if ((val = g_config->get("language")))
		if ((_language = parseLanguage(val)) == -1) {
			printf("Error in the config file: invalid language.\n");
			printf(USAGE_STRING);
			exit(-1);
		}

	_master_volume = g_config->getInt("master_volume", _master_volume);

	_music_volume = g_config->getInt("music_volume", _music_volume);

	_noSubtitles = g_config->getBool("nosubtitles", _noSubtitles ? true : false);

	if ((val = g_config->get("path")))
		_gameDataPath = strdup(val);

	_sfx_volume = g_config->getInt("sfx_volume", _sfx_volume);

	_debugLevel = g_config->getInt("debuglevel", _debugLevel);
	if (_debugLevel > 0) {
		_debugMode = true;
		debug(1, "Debuglevel (from config): %d", _debugLevel);
	}

	// We use strtol for the tempo to allow it to be specified in hex.
	if ((val = g_config->get("tempo")))
		_gameTempo = strtol(val, NULL, 0);

	_talkSpeed = g_config->getInt("talkspeed", _talkSpeed);

	_multi_midi = g_config->getBool ("multi_midi");
	_native_mt32 = g_config->getBool ("native_mt32");
}

void GameDetector::list_games() {
	const VersionSettings *v = version_settings;
	const char *config;

	printf("Game          Full Title                                              Config\n"
	       "------------- ------------------------------------------------------- -------\n");

	while (v->filename && v->gamename) {
		config = (g_config->has_domain(v->filename)) ? "Yes" : "";
		printf("%-14s%-57s%s\n", v->filename, v->gamename, config);
		v++;
	}
		
}

void GameDetector::parseCommandLine(int argc, char **argv) {
	int i;
	char *s;
	char *current_option = NULL;
	char *option = NULL;
	char c;
	_save_slot = -1;

	// Parse the arguments
	// into a transient "_COMMAND_LINE" config comain.
	g_config->set_domain ("_COMMAND_LINE");
	for (i = argc - 1; i >= 1; i--) {
		s = argv[i];

		if (s[0] == '-') {
			s++;
			c = *s++;
			switch (tolower(c)) {
			case 'a':
				CHECK_OPTION();
				_amiga = (c == 'a');
				g_config->setBool("amiga", _amiga);
				break;
			case 'b':
				HANDLE_OPTION();
				_bootParam = atoi(option);
				break;
			case 'c':
				HANDLE_OPTION();
				_cdrom = atoi(option);
				g_config->setInt("cdrom", _cdrom);
				break;
			case 'd':
				_debugMode = true;
				HANDLE_OPT_OPTION();
				if (option != NULL)
					_debugLevel = atoi(option);
				debug(1,"Debuglevel (from command line): %d", _debugLevel);
				break;
			case 'e':
				HANDLE_OPTION();
				if (!parseMusicDriver(option))
					goto ShowHelpAndExit;
				g_config->set("music_driver", option);
				break;
			case 'f':
				CHECK_OPTION();
				_fullScreen = (c == 'f');
				g_config->setBool("fullscreen", _fullScreen, "scummvm");
				break;
			case 'g':
				HANDLE_OPTION();
				_gfx_mode = parseGraphicsMode(option);
				if (_gfx_mode == -1)
					goto ShowHelpAndExit;
				g_config->set("gfx_mode", option, "scummvm");
				break;
			// case 'h': reserved for help
			// case 'j': reserved for joystick select
			case 'l':
				HANDLE_OPTION();
				{
					Config *newconfig = new Config(option, "scummvm");
					g_config->merge_config(*newconfig);
					delete newconfig;
					updateconfig();
					break;
				}
				break;
			case 'm':
				HANDLE_OPTION();
				_music_volume = atoi(option);
				g_config->setInt("music_volume", _music_volume);
				break;
			case 'n':
				CHECK_OPTION();
				_noSubtitles = (c == 'n');
				g_config->setBool("nosubtitles", _noSubtitles ? true : false);
				break;
 			case 'o':
 				HANDLE_OPTION(); 
 				_master_volume = atoi(option); 
 				g_config->setInt("master_volume", _master_volume); 
 				break; 
			case 'p':
				HANDLE_OPTION();
				_gameDataPath = option;
				g_config->set("path", _gameDataPath);
				break;
			case 'q':
				HANDLE_OPTION();
				_language = parseLanguage(option);
				if (_language == -1)
					goto ShowHelpAndExit;
				g_config->set("language", option);
				break;
			case 'r':
				HANDLE_OPTION();
				// Ignore -r for now, to ensure backward compatibility.
				break;
			case 's':
				HANDLE_OPTION();
				_sfx_volume = atoi(option);
				g_config->setInt("sfx_volume", _sfx_volume);
				break;
			case 't':
				HANDLE_OPTION();
				_gameTempo = strtol(option, 0, 0);
				g_config->set("tempo", option);
				break;
			case 'u':
				CHECK_OPTION();
				_dumpScripts = true;
				break;
			case 'v':
				CHECK_OPTION();
				printf("ScummVM " SCUMMVM_VERSION "\nBuilt on " __DATE__ " "
							 __TIME__ "\n");
				exit(1);
			case 'w':
				_saveconfig = true;
				g_config->set_writing(true);
				HANDLE_OPT_OPTION();
				if (option != NULL)
					g_config->set_filename(option);
				break;
			case 'x':
				_save_slot = 0;
				HANDLE_OPT_OPTION();
				if (option != NULL) {
					_save_slot = atoi(option);
					g_config->setInt("save_slot", _save_slot);
				}
				break;
			case 'y':
				HANDLE_OPTION();
				_talkSpeed = atoi(option);
				g_config->setInt("talkspeed", _talkSpeed);
				break;
			case 'z':
				CHECK_OPTION();
				list_games();
				exit(1);
			case '-':
				// Long options. Let the fun begin!
				if (!strcmp (s, "multi-midi")) {
					_multi_midi = true;
					g_config->setBool ("multi_midi", true);
				} else if (!strcmp (s, "native-mt32")) {
					_native_mt32 = true;
					g_config->setBool ("native_mt32", true);
				} else {
					goto ShowHelpAndExit;
				}
				break;
			default:
				goto ShowHelpAndExit;
			}
		} else {
			if (i == (argc - 1)) {
				setGame(s);
			} else {
				if (current_option == NULL)
					current_option = s;
				else
					goto ShowHelpAndExit;
			}
		}
	}
	
	if (!_gameFileName.isEmpty())
		g_config->flush();

	return;

ShowHelpAndExit:
	printf(USAGE_STRING);
	exit(1);
}

void GameDetector::setGame(const String &name) {
	_gameFileName = name;
	g_config->set_domain(name);
	g_config->rename_domain(name, "game-specific");
	g_config->rename_domain("game-specific", name);
	updateconfig();

	// The command line and launcher options
	// override config file global and game-specific options.
	g_config->set_domain ("_COMMAND_LINE");
	updateconfig();
	g_config->set_domain ("_USER_OVERRIDES");
	updateconfig();
	g_config->delete_domain ("_COMMAND_LINE");
	g_config->delete_domain ("_USER_OVERRIDES");
	g_config->set_domain(name);
}

int GameDetector::parseGraphicsMode(const char *s) {
	const GraphicsMode *gm = gfx_modes;
	while(gm->name) {
		if (!scumm_stricmp(gm->name, s)) {
			_default_gfx_mode = false;
			return gm->id;
		}
		gm++;
	}

	return -1;
}

int GameDetector::parseLanguage(const char *s) {
	const Language *l = languages;
	while(l->name) {
		if (!scumm_stricmp(l->name, s))
			return l->id;
		l++;
	}

	return -1;
}

bool GameDetector::isMusicDriverAvailable(int drv) {
	switch(drv) {
	case MD_AUTO:
	case MD_NULL: return true;
#ifndef __PALM_OS__	// not avalaible on palmos : Clie use only ADPCM data and cannot be converted on the fly, may be possible on TT ?
	case MD_ADLIB: return true;
#else
	case MD_YPA1: return true;
#endif
#if defined(WIN32) && !defined(_WIN32_WCE)
	case MD_WINDOWS: return true;
#endif
#if defined(__MORPHOS__)
	case MD_ETUDE: return true;
#endif
#if defined(UNIX) && !defined(__BEOS__) && !defined(MACOSX)
	case MD_SEQ: return true;
#endif
#if defined(MACOSX) || defined(macintosh)
	case MD_QTMUSIC: return true;
#endif
#if defined(MACOSX)
	case MD_COREAUDIO: return true;
#endif
#if defined(UNIX) && defined(USE_ALSA)
	case MD_ALSA: return true;
#endif
	}
	return false;
}

const MusicDriver *GameDetector::getMusicDrivers() {
	return music_drivers;
}

bool GameDetector::parseMusicDriver(const char *s) {
	const MusicDriver *md = music_drivers;

	while (md->name) {
		if (!scumm_stricmp(md->name, s)) {
			_use_adlib = (md->id == MD_ADLIB);
			_midi_driver = md->id;
			return true;
		}
		md++;
	}

	return false;
}

bool GameDetector::detectGame() {
	const VersionSettings *gnl = version_settings;
	const char *realGame, *basename;
	_game.id = 0;
	_gameText.clear();

	realGame = g_config->get("gameid");
	if (!realGame)
		realGame = _gameFileName.c_str();
	printf("Looking for %s\n", realGame);

	do {
		if (!scumm_stricmp(realGame, gnl->filename)) {
			_game = *gnl;
			if ((basename = g_config->get("basename")))	{
				// FIXME: What is this good for?
				_game.filename = basename;
			}
			_gameText = gnl->gamename;
			debug(1, "Trying to start game '%s'",gnl->gamename);
			return true;
		}
	} while ((++gnl)->filename);

	debug(1, "Failed game detection");

	return false;
}

const ScummVM::String& GameDetector::getGameName() {
	if (_gameText.isEmpty()) {
		_gameText = "Unknown game: \"";
		_gameText += _gameFileName;
		_gameText += "\"";
	}
	return _gameText;
}

int GameDetector::detectMain() {
	if (_gameFileName.isEmpty()) {
		warning("No game was specified...");
		return (-1);
	}

	if (!detectGame()) {
		warning("Game detection failed. Using default settings");
		_gameText = "Please choose a game";
	}

	/* Use the adlib sound driver if auto mode is selected,
	 * and the game is one of those that want adlib as
	 * default, OR if the game is an older game that doesn't
	 * support anything else anyway. */
#ifndef __PALM_OS__ // currently adlib is not supported on PalmOS
	if ((_game.adlib & VersionSettings::ADLIB_ALWAYS) ||
	   ((_game.adlib & VersionSettings::ADLIB_PREFERRED) && _midi_driver == MD_AUTO)) {
		_midi_driver = MD_ADLIB;
		_use_adlib = true;
	}
#endif

	if (!_gameDataPath) {
		warning("No path was provided. Assuming the data files are in the current directory");
		_gameDataPath = "";
#ifndef __PALM_OS__	// add last slash also in File::fopenNoCase, so this is not needed
	} else if (_gameDataPath[strlen(_gameDataPath)-1] != '/'
#ifdef __MORPHOS__
					&& _gameDataPath[strlen(_gameDataPath)-1] != ':'
#endif
					&& _gameDataPath[strlen(_gameDataPath)-1] != '\\') {
		char slashless[1024];	/* Append slash to path */
		strcpy(slashless, _gameDataPath);
		
		// need to allocate 2 extra bytes, one for the "/" and one for the NULL terminator
		_gameDataPath = (char *)malloc((strlen(slashless) + 2) * sizeof(char));
		sprintf(_gameDataPath, "%s/", slashless);
#endif
	}

	return (0);
}

OSystem *GameDetector::createSystem() {
#if defined(USE_NULL_DRIVER)
	return OSystem_NULL_create();
#elif defined(__DC__)
	return OSystem_Dreamcast_create();
#elif defined(X11_BACKEND)
	return OSystem_X11_create();
#elif defined(__MORPHOS__)
	return OSystem_MorphOS_create(_game.id, _gfx_mode, _fullScreen);
#elif defined(_WIN32_WCE)
	return OSystem_WINCE3_create();
#elif defined(MACOS_CARBON)
	return OSystem_MAC_create(_gfx_mode, _fullScreen);
#elif defined(__GP32__)	// ph0x
	return OSystem_GP32_create(GFX_NORMAL, true);
#elif defined(__PALM_OS__) //chrilith
	return OSystem_PALMOS_create(_gfx_mode);
#else
	/* SDL is the default driver for now */
	return OSystem_SDL_create(_gfx_mode, _fullScreen);
#endif
}

int GameDetector::getMidiDriverType() {

	if (_midi_driver != MD_AUTO) return _midi_driver;

#if defined (WIN32) && !defined(_WIN32_WCE)
		return MD_WINDOWS; // MD_WINDOWS is default MidiDriver on windows targets
#elif defined(MACOSX)
		return MD_COREAUDIO;
#elif defined(__PALM_OS__)	// must be before mac
		return MD_YPA1;
#elif defined(macintosh)
		return MD_QTMUSIC;
#elif defined(__MORPHOS__)
		return MD_ETUDE;
#elif defined (_WIN32_WCE) || defined(UNIX) || defined(X11_BACKEND)
	// Always use MIDI emulation via adlib driver on CE and UNIX device

	// TODO: We should, for the Unix targets, attempt to detect
	// whether a sequencer is available, and use it instead.
	return MD_ADLIB;
#endif
    return MD_NULL;
}

MidiDriver *GameDetector::createMidi() {
	int drv = getMidiDriverType();

	switch(drv) {
	case MD_NULL:		return MidiDriver_NULL_create();
#ifndef __PALM_OS__
	// In the case of Adlib, we won't specify anything.
	// IMuse is designed to set up its own Adlib driver
	// if need be, and we only have to specify a native
	// driver.
	case MD_ADLIB:		_use_adlib = true; return NULL;
#else
	case MD_YPA1:		return MidiDriver_YamahaPa1_create();
#endif
#if defined(WIN32) && !defined(_WIN32_WCE)
	case MD_WINDOWS:	return MidiDriver_WIN_create();
#endif
#if defined(__MORPHOS__)
	case MD_ETUDE:		return MidiDriver_ETUDE_create();
#endif
#if defined(UNIX) && !defined(__BEOS__) && !defined(MACOSX)
	case MD_SEQ:		return MidiDriver_SEQ_create();
#endif
#if (defined(MACOSX) || defined(macintosh)) && !defined(__PALM_OS__)
	case MD_QTMUSIC:	return MidiDriver_QT_create();
#endif
#if defined(MACOSX)
	case MD_COREAUDIO:	return MidiDriver_CORE_create();
#endif
#if defined(UNIX) && defined(USE_ALSA)
	case MD_ALSA:		return MidiDriver_ALSA_create();
#endif
	}

	error("Invalid midi driver selected");
	return NULL;
}
