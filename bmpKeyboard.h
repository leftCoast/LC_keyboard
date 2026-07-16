#ifndef bmpKeyboard_h
#define bmpKeyboard_h

#include <scrKeyboard.h>
#include <bmpObj.h>
#include <bitmap.h>

// *****************************************************
//                       screenTypes
// *****************************************************


enum screenTypes {
	sType240x320,
	sType320x480
};



// *****************************************************
//                       bmpKeyboard
// *****************************************************


class bmpKeyboard : public keyboard {

	public:
				bmpKeyboard(editable* inEditObj,bool modal=false,screenTypes=sType240x320);
	virtual	~bmpKeyboard(void);

				void			initParams(screenTypes inDispType);
				char* 		getBmpPath(char* bmpName);
				int			col(int col, int row);
				int			row(int inRow);
				bitmap*		getKeyMap(void);
				colorObj*	getKeyTextColor(void);
	virtual	void			loadKeys(void);
  
				bool    	modal;
				bitmap	keyBMap;
				colorObj	keyTextColor;
          	int		keyWidth;
          	int		keyHeight;
          	char*		pathBuff;
				char*		bmpFolder;
				char*		checkBmp;
				char*		deleteBmp;
				char*		keyCapBmp;
				char*		lArrowBmp;
				char*		rArrowBmp;
				char*		returnBmp;
				char*		shiftBmp;
				char*		spaceBmp;
				char*		symbolBmp;
				char*		redXBmp;
				int		textXOffset;
				int		textYOffset;
};



// *****************************************************
//                       bmpInputKey
// *****************************************************


class bmpInputKey :  public inputKey {

	public:
				bmpInputKey(const char* inLabel,const char* inNum,const char* inSym,int inX,int inY,int inWidth,int inHeight,bmpKeyboard* inKeyboard);
	virtual	~bmpInputKey(void);

	virtual	void	drawSelf(void);
  				
  				bmpKeyboard*	ourKeyboard;
				bitmap*			keyBMap;
};



// *****************************************************
//                       bmpControlKey
// *****************************************************


class bmpControlKey : public controlKey {

  public:
          bmpControlKey(const char* inLabel,keyCommands inCom,int inX, int inY,int inWidth,int inHeight,bmpKeyboard* inKeyboard,const char* bmpPath);
  virtual ~bmpControlKey(void);

  virtual void    drawSelf(void);
          
          bmpObj*	ourBMPObj;
};


#endif
