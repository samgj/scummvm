/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef FULLPIPE_MOTION_H
#define FULLPIPE_MOTION_H

namespace Fullpipe {

class Statics;
class Movement;
class MctlConnectionPoint;

int startWalkTo(int objId, int objKey, int x, int y, int a5);
int doSomeAnimation(int objId, int objKey, int a3);
int doSomeAnimation2(int objId, int objKey);

class MotionController : public CObject {
public:
	int _field_4;
	bool _isEnabled;

public:
	MotionController() : _isEnabled(true), _field_4(0) {}
	virtual ~MotionController() {}
	virtual bool load(MfcArchive &file);
	virtual void methodC() {}
	virtual void method10() {}
	virtual void clearEnabled() { _isEnabled = false; }
	virtual void setEnabled() { _isEnabled = true; }
	virtual void addObject(StaticANIObject *obj) {}
	virtual int removeObject(StaticANIObject *obj) { return 0; }
	virtual void freeItems() {}
	virtual int method28() { return 0; }
	virtual int method2C() { return 0; }
	virtual int method30() { return 0; }
	virtual MessageQueue *method34(StaticANIObject *subj, int xpos, int ypos, int fuzzyMatch, int staticsId) { return 0; }
	virtual int changeCallback() { return 0; }
	virtual int method3C() { return 0; }
	virtual int method40() { return 0; }
	virtual int method44() { return 0; }
	virtual int method48() { return -1; }
	virtual MessageQueue *doWalkTo(StaticANIObject *subj, int xpos, int ypos, int fuzzyMatch, int staticsId) { return 0; }
};

class MovGraphReact : public CObject {
public:
	int _pointCount;
	Common::Point **_points;

public:
	MovGraphReact() : _pointCount(0), _points(0) {}
	~MovGraphReact() { free(_points); }

	virtual void method14() {}
	virtual void createRegion() {}
	virtual bool pointInRegion(int x, int y);
};

class MctlCompoundArrayItem : public CObject {
	friend class MctlCompound;

  protected:
	MotionController *_motionControllerObj;
	MovGraphReact *_movGraphReactObj;
	Common::Array<MctlConnectionPoint *> _connectionPoints;
	int _field_20;
	int _field_24;
	int _field_28;

 public:
	MctlCompoundArrayItem() : _movGraphReactObj(0) {}
};

class MctlCompoundArray : public Common::Array<MctlCompoundArrayItem *>, public CObject {
 public:
	virtual bool load(MfcArchive &file);
};

class MctlCompound : public MotionController {
	MctlCompoundArray _motionControllers;

 public:
	MctlCompound() { _objtype = kObjTypeMctlCompound; }

	virtual bool load(MfcArchive &file);

	virtual void addObject(StaticANIObject *obj);
	virtual int removeObject(StaticANIObject *obj);
	virtual void freeItems();
	virtual MessageQueue *method34(StaticANIObject *subj, int xpos, int ypos, int fuzzyMatch, int staticsId);
	virtual MessageQueue *doWalkTo(StaticANIObject *subj, int xpos, int ypos, int fuzzyMatch, int staticsId);

	void initMovGraph2();
	MctlConnectionPoint *findClosestConnectionPoint(int ox, int oy, int destIndex, int connectionX, int connectionY, int sourceIndex, int *minDistancePtr);
};

struct MGMSubItem {
	int movement;
	int staticsIndex;
	int field_8;
	int field_C;
	int x;
	int y;

	MGMSubItem();
};

struct MGMItem {
	int16 objId;
	Common::Array<MGMSubItem *> subItems;
	Common::Array<Statics *> statics;
	Common::Array<Movement *> movements1;
	Common::Array<Movement *> movements2;

	MGMItem();
};


class MGM : public CObject {
public:
	Common::Array<MGMItem *> _items;

public:
	void clear();
	void addItem(int objId);
	void rebuildTables(int objId);
	int getItemIndexById(int objId);
};

class MovGraphNode : public CObject {
public:
	int _x;
	int _y;
	int _distance;
	int16 _field_10;
	int _field_14;

public:
	MovGraphNode() : _x(0), _y(0), _distance(0), _field_10(0), _field_14(0) { _objtype = kObjTypeMovGraphNode; }
	virtual bool load(MfcArchive &file);
};

class ReactParallel : public MovGraphReact {
	//CRgn _rgn;
	int _x1;
	int _y1;
	int _x2;
	int _y2;
	int _dx;
	int _dy;

  public:
	ReactParallel();
	virtual bool load(MfcArchive &file);

	virtual void method14();
	virtual void createRegion();
};

class ReactPolygonal : public MovGraphReact {
	//CRgn _rgn;
	int _field_C;
	int _field_10;

  public:
	ReactPolygonal();
	virtual bool load(MfcArchive &file);

	virtual void method14();
	virtual void createRegion();
};

class MovGraphLink : public CObject {
 public:
	MovGraphNode *_movGraphNode1;
	MovGraphNode *_movGraphNode2;
	DWordArray _dwordArray1;
	DWordArray _dwordArray2;
	int _flags;
	int _field_38;
	int _field_3C;
	double _distance;
	double _angle;
	MovGraphReact *_movGraphReact;
	char *_name;

  public:
	MovGraphLink();
	virtual bool load(MfcArchive &file);

	void calcNodeDistanceAndAngle();
};

struct MovGraphItem {
	StaticANIObject *ani;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
	int items;
	int count;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;

	MovGraphItem();
};

class MovGraph : public MotionController {
 public:
	ObList _nodes;
	ObList _links;
	int _field_44;
	Common::Array<MovGraphItem *> _items;
	int (*_callback1)(int, int, int);
	MGM _mgm;

 public:
	MovGraph();
	virtual bool load(MfcArchive &file);

	virtual void addObject(StaticANIObject *obj);
	virtual int removeObject(StaticANIObject *obj);
	virtual void freeItems();
	virtual int method28();
	virtual int method2C();
	virtual MessageQueue *method34(StaticANIObject *subj, int xpos, int ypos, int fuzzyMatch, int staticsId);
	virtual int changeCallback();
	virtual int method3C();
	virtual int method44();
	virtual MessageQueue *doWalkTo(StaticANIObject *subj, int xpos, int ypos, int fuzzyMatch, int staticsId);
	virtual int method50();

	double calcDistance(Common::Point *point, MovGraphLink *link, int fuzzyMatch);
	void calcNodeDistancesAndAngles();
	MovGraphNode *calcOffset(int ox, int oy);
};

class Movement;

struct MG2I {
	int _movementId;
	Movement *_mov;
	int _mx;
	int _my;
};

struct MovGraph2ItemSub {
	int _staticsId2;
	int _staticsId1;
	MG2I _walk[3];
	MG2I _turn[4];
	MG2I _turnS[4];
};

struct LinkInfo {
	MovGraphLink *link;
	MovGraphNode *node;
};

struct MovInfo1Sub {
	int subIndex;
	int x;
	int y;
	int distance;
};

struct MovInfo1 {
	int field_0;
	Common::Point pt1;
	Common::Point pt2;
	int distance1;
	int distance2;
	int subIndex;
	int item1Index;
	Common::Array<MovInfo1Sub *> items;
	int itemsCount;
	int flags;
};

struct MovGraph2Item {
	int _objectId;
	StaticANIObject *_obj;
	MovGraph2ItemSub _subItems[4];
};

class MovGraph2 : public MovGraph {
public:
	Common::Array<MovGraph2Item *> _items;

public:
	virtual void addObject(StaticANIObject *obj);
	virtual int removeObject(StaticANIObject *obj);
	virtual void freeItems();
	virtual MessageQueue *method34(StaticANIObject *subj, int xpos, int ypos, int fuzzyMatch, int staticsId);
	virtual MessageQueue *doWalkTo(StaticANIObject *subj, int xpos, int ypos, int fuzzyMatch, int staticsId);

	int getItemIndexByGameObjectId(int objectId);
	int getItemSubIndexByStaticsId(int index, int staticsId);
	int getItemSubIndexByMovementId(int index, int movId);
	int getItemSubIndexByMGM(int idx, StaticANIObject *ani);

	int getShortSide(MovGraphLink *lnk, int x, int y);
	int findLink(Common::Array<MovGraphLink *> *linkList, int idx, Common::Rect *a3, Common::Point *a4);

	bool initDirections(StaticANIObject *obj, MovGraph2Item *item);
	void buildMovInfo1SubItems(MovInfo1 *movinfo, Common::Array<MovGraphLink *> *linkList, LinkInfo *lnkSrc, LinkInfo *lnkDst);
	MessageQueue *buildMovInfo1MessageQueue(MovInfo1 *movInfo);

	MovGraphNode *findNode(int x, int y, int fuzzyMatch);
	MovGraphLink *findLink1(int x, int y, int idx, int fuzzyMatch);
	MovGraphLink *findLink2(int x, int y);
	double findMinPath(LinkInfo *linkInfoSource, LinkInfo *linkInfoDest, Common::Array<MovGraphLink *> *listObj);
};

class MctlConnectionPoint : public CObject {
public:
	int _connectionX;
	int _connectionY;
	int _field_C;
	int _field_10;
	int16 _field_14;
	int16 _field_16;
	MessageQueue *_messageQueueObj;
	int _motionControllerObj;
};

} // End of namespace Fullpipe

#endif /* FULLPIPE_MOTION_H */
