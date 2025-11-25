#ifndef scrKeyboard_h
#define scrKeyboard_h

#include <drawObj.h>
#include <label.h>
#include <keystroke.h>
#include <editable.h>
#include <idlers.h>


// ************************************************************
//                     legacy drawing code.
// ************************************************************


// stampObj, is a lightweight package of drawing commands. stamObj
// DOES NOT rely on drawObj or its drawing environment to function.
// This means it can be used in simple Arduino sketches that don't
// incorporate the drawObj drawing framework. 

// When creating drawing commands for a stampObj, its important to
// use it's internal x,y coordinates as your origin. Then, whomever
// uses your stamp object, can position it anywhere on the screen.
// This also allows it to seamlessly function IN the drawObj drawing
// environment, as an add in by other drawObj(s) for their drawSelf()
// methods.
//
// Cheap, reusable, flexible, simple to create and use.. Who wouldn't
// love it?!
//
// So then.. It works perfectly well in or out of the drawing environment,
// what is the difference between a stampObj and a drawObj?
//
// Both drawObj(s) and stampObj(s) use the global screenObj, "screen" to do
// their drawing. The difference lie in the fact that drawObject(s) "live"
// in an automated redraw system. Things can effect them that will causes
// their needRefresh flag to be set. All of their drawing is done
// automatically "behind the scenes" during idle time. Along with this
// automatic refresh system, they also support groups and subgroups with
// all the management that goes along with it.
// 
// stampObj(s) have none of this automated management. They are just packs
// of drawing commands. Just like a stamp, you aim it at the screen and
// stamp out its drawing commands, Bam! You still use the global screen
// object to do the actual drawing, but that's about it.
//
// Want a smily face icon here or there? Create a stamp for it and you can 
// stamp smily faces wherever on the screen you like. Or, you can add it into
// a drawObj's drawSelf() method and have that as part of its drawing package.


class stampObj :	public rect {

	public:
				stampObj(rect* aRect);
	virtual	~stampObj(void);
	
	virtual	void stamp(void);
};


// ************************************************************


class drawDelete : public stampObj {

  public:
          drawDelete(rect* inRect);
  virtual ~drawDelete(void);
          
          void setColors(colorObj* foreColor,colorObj* backColor);
  virtual void stamp(void);

          colorObj  mForeColor;
          colorObj  mBackColor;
};


// ************************************************************


class enterArrow : public stampObj {

  public:
          enterArrow(rect* inRect);
  virtual ~enterArrow(void);
          
          void setColors(colorObj* foreColor,colorObj* backColor);
  virtual void stamp(void);

          colorObj  mForeColor;
          colorObj  mBackColor;
};


// ************************************************************
//                      End old drawing code.
// ************************************************************



// Keys need to know the state of the keyboard. When they get time
// they'll check the keyboard to see what state things are in and
// redraw themselves accordingly. Also output their data accordingly.

enum  keyStates { chars, shifted, numbers, symbols };

class keyboard;

struct keyColors {
	colorObj		inputKeyText;
	colorObj		inputKeyBase;
	colorObj		inputKeyHText;
	colorObj		inputKeyHBase;
	
	colorObj		contolKeyText;
	colorObj		contolKeyBase;
	colorObj		contolKeyHText;
	colorObj		contolKeyHBase;
	
	colorObj		deleteKeyText;
	colorObj		deleteKeyBase;
	colorObj		deleteKeyHText;
	colorObj		deleteKeyHBase;
};

extern keyColors kbPallette;


class keyboardKey : public idler {

	public:
						keyboardKey(keyboard* inKeyboard);
	virtual			~keyboardKey(void);
	
	virtual	void	beenClicked(void);
	
				keyboard*   mKeyboard;  // Master!
				keyStates   mState;     // Current state I'm showing.
				char        mChar;
				char        mNum;
				char        mSymbol;
				keyCommands mCom;
};


// inputKeys are for inputting data. Like letters & numbers.
class inputKey : public keyboardKey, public label {

  public:
          inputKey(const char* inLabel,const char* inNum,const char* inSym,word locX, word locY,byte width,byte height,keyboard* inKeyboard);
  virtual ~inputKey(void);

  virtual void    idle();     // Use this to keep updated.
  virtual void    drawSelf(void);
  virtual void    doAction(void);
};


// controlKeys are for doing everything else. Like shift, delete and arrow keys.
class controlKey : public keyboardKey, public label {

  public:
          controlKey(const char* inLabel,keyCommands inCom,word locX, word locY,byte width,byte height,keyboard* inKeyboard);
  virtual ~controlKey(void);

  virtual void    drawSelf(void);
  virtual void    handleShift(void); 
  virtual void    handleNumber(void);
  virtual void    doAction(void);
};


class keyboard : public drawGroup {

	public:
				keyboard(editable* inEditObj=NULL);
	virtual	~keyboard(void);

	virtual	void			loadKeys(void);
	virtual	void			keyClicked(keyboardKey* aKey);
	virtual	void			handleKey(char inChar);
	virtual	void			handleKey(keyCommands inEditCom);
	virtual	void			handleKey(keyStates inState);
	virtual	void			setEditField(editable* inLabel);
	virtual	editable*	getEditField(void);
	virtual	keyStates	getState(void);
	virtual	void    		drawSelf(void);
			
          editable*		mEditObj;
          keyStates		mState;
}; 


#endif
