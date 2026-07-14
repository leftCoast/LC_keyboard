#include <bmpKeyboard.h>
#include <editable.h>
#include <bitmap.h>
#include <offscreen.h>
#include <resizeBuff.h>
#include <strTools.h>



#define ICON_PATH		"/system/icons/keyboard/"


// *****************************************************
//                       bmpKeyboard
// *****************************************************


bmpKeyboard::bmpKeyboard(editable* inEditObj,bool inModal,screenTypes inType)
  : keyboard (inEditObj) {

  	rect			sRect;
	offscreen	vPort; 
	bmpObj*		keyCap;
	Serial.println("Trying to make a keyboard?");
	pathBuff		= NULL;																	// Set all our strings to NULL..
	bmpFolder	= NULL;																	//
	checkBmp		= NULL;																	//
	deleteBmp	= NULL;																	//
	keyCapBmp	= NULL;																	//
	lArrowBmp	= NULL;																	//
	rArrowBmp	= NULL;																	//
	returnBmp	= NULL;																	//
	shiftBmp		= NULL;																	//
	spaceBmp		= NULL;																	//
	symbolBmp	= NULL;																	//
	redXBmp		= NULL;																	//
	initParams(inType);																	// Setup the sizes & choices of stuff to fit this screen.
	keyTextColor.setColor(LC_YELLOW);												// Set up key text color.
	keyTextColor.blend(&white,60);													// Little adjustment here..
	modal = inModal;																		// Note if we are modal or not.
	keyCap = new bmpObj(0,0,keyWidth,keyHeight,getBmpPath(keyCapBmp));	// Create our bitmap drawing object.
	if (keyCap) {																			// If we got one..
		if (keyBMap.setSize(keyWidth,keyHeight)) {										// If we can allocate the RAM for the icon's bitmap..
			vPort.beginDraw(&keyBMap);													// Set up to offscreen drawing to the bitmap.
			keyCap->drawSelf(); 															// Draw to it..
			vPort.endDraw();																// Restore normal drawing.
		}																						//
		delete(keyCap);																	// Recycle the bitmap image object.
	}																							//
}																								//


bmpKeyboard::~bmpKeyboard(void) {

	keyBMap.clearMap();
	freeStr(&pathBuff);
	freeStr(&bmpFolder);
	freeStr(&checkBmp);
	freeStr(&deleteBmp);
	freeStr(&keyCapBmp);
	freeStr(&lArrowBmp);
	freeStr(&rArrowBmp);
	freeStr(&returnBmp);
	freeStr(&shiftBmp);
	freeStr(&spaceBmp);
	freeStr(&symbolBmp);
	freeStr(&redXBmp);
}


void bmpKeyboard::initParams(screenTypes inDispType) {
	
	int screenH;
	
	heapStr(&bmpFolder,ICON_PATH);
	switch (inDispType) {
		case sType240x320	:
			keyWidth		= 24;
			keyHeight	= 33;
			screenH		= 320;
			heapStr(&deleteBmp,"delete36.bmp");
			heapStr(&keyCapBmp,"keyCap24.bmp");
			heapStr(&lArrowBmp,"lArrow24.bmp");
			heapStr(&rArrowBmp,"rArrow24.bmp");
			heapStr(&shiftBmp,"shift36.bmp");
			heapStr(&spaceBmp,"spaceB72.bmp");
			if (modal) {
				heapStr(&checkBmp,"check36.bmp");
				heapStr(&symbolBmp,"symb24.bmp");
				heapStr(&redXBmp,"x36.bmp");
			} else {
				heapStr(&returnBmp,"ret36.bmp");
				heapStr(&symbolBmp,"symb36.bmp");
			}
			setRect(0, screenH - 4 * keyHeight, 240, 4 * keyHeight);	// Set our rectangle.
		break;
		case sType320x480	:
			keyWidth		= 32;
			keyHeight	= 44;
			screenH		= 360;
			heapStr(&deleteBmp,"delete48.bmp");
			heapStr(&keyCapBmp,"keyCap32.bmp");
			heapStr(&lArrowBmp,"lArrow32.bmp");
			heapStr(&rArrowBmp,"rArrow32.bmp");
			heapStr(&shiftBmp,"shift48.bmp");
			heapStr(&spaceBmp,"spaceB96.bmp");
			if (modal) {
				heapStr(&checkBmp,"check48.bmp");
				heapStr(&symbolBmp,"symb32.bmp");
				heapStr(&redXBmp,"x48.bmp");
			} else {
				heapStr(&returnBmp,"ret48.bmp");
				heapStr(&symbolBmp,"symb48.bmp");
			}
			setRect(0, screenH - 4 * keyHeight, 240, 4 * keyHeight);	// Set our rectangle.
		break;
		default				:														// Default to original. 240x360
			keyWidth		= 24;
			keyHeight	= 33;
			screenH		= 360;
			heapStr(&deleteBmp,"delete36.bmp");
			heapStr(&keyCapBmp,"keyCap24.bmp");
			heapStr(&lArrowBmp,"lArrow24.bmp");
			heapStr(&rArrowBmp,"rArrow24.bmp");
			heapStr(&shiftBmp,"shift36.bmp");
			heapStr(&spaceBmp,"spaceB72.bmp");
			if (modal) {
				heapStr(&checkBmp,"check36.bmp");
				heapStr(&symbolBmp,"symb24.bmp");
				heapStr(&redXBmp,"x36.bmp");
			} else {
				heapStr(&returnBmp,"ret36.bmp");
				heapStr(&symbolBmp,"symb36.bmp");
			}
			setRect(0, screenH - 4 * keyHeight, 240, 4 * keyHeight);	// Set our rectangle.
		break;
	}
}


// Put together and return the correct path name for this bitmap file. Return NULL on any
// error.
char* bmpKeyboard::getBmpPath(char* bmpName) {

	int	numChars;
	
	if (bmpFolder && bmpName) {		// Sanity..
		numChars = strlen(bmpFolder);
		numChars = numChars + strlen(bmpName);
		numChars++;
		if (resizeBuff(numChars,&pathBuff)) {
			strcpy(pathBuff,bmpFolder);
			strcat(pathBuff,bmpName);
			return(pathBuff);
		}
	}
	return NULL;
}
	
	
		
// Given the colum I'm shooting for and the row I'm in.. 
// What's the colom pixel I start on?
int bmpKeyboard::col(int col, int row) {

  switch (row) {
    case 1 : return keyWidth * (col - 1);
    case 2 : return keyWidth * ((col-1) + 0.5);
    case 3 : 
      if (col==1) return 0;
      if (col>1&&col<10) return keyWidth * ((col-1) + 0.5);
      return keyWidth * 9;
    case 4 : if (modal) {
        if (col==9) return keyWidth * ((col-1) + 0.5);
        return keyWidth * (col - 1);
      } else {
        if (col>1) return keyWidth * ((col-1) + 0.5);
        return keyWidth * (col - 1);
      }
  }
  return 0;	// Shut up compiler!
}


int bmpKeyboard::row(int inRow) { return keyHeight * (inRow - 1); }


bitmap* bmpKeyboard::getKeyMap(void) { return &keyBMap; }


colorObj* bmpKeyboard::getKeyTextColor(void) { return &keyTextColor; }


void bmpKeyboard::loadKeys(void) {

  bmpInputKey* qKey = new bmpInputKey("q", "1", "", col(1,1), row(1), keyWidth, keyHeight, this);
  bmpInputKey* wKey = new bmpInputKey("w", "2", "", col(2,1), row(1), keyWidth, keyHeight, this);
  bmpInputKey* eKey = new bmpInputKey("e", "3", "", col(3,1), row(1), keyWidth, keyHeight, this);
  bmpInputKey* rKey = new bmpInputKey("r", "4", "", col(4,1), row(1), keyWidth, keyHeight, this);
  bmpInputKey* tKey = new bmpInputKey("t", "5", "", col(5,1), row(1), keyWidth, keyHeight, this);
  bmpInputKey* yKey = new bmpInputKey("y", "6", "", col(6,1), row(1), keyWidth, keyHeight, this);
  bmpInputKey* uKey = new bmpInputKey("u", "7", "", col(7,1), row(1), keyWidth, keyHeight, this);
  bmpInputKey* iKey = new bmpInputKey("i", "8", "", col(8,1), row(1), keyWidth, keyHeight, this);
  bmpInputKey* oKey = new bmpInputKey("o", "9", "", col(9,1), row(1), keyWidth, keyHeight, this);
  bmpInputKey* pKey = new bmpInputKey("p", "0", "", col(10,1), row(1), keyWidth, keyHeight, this);

  addObj(qKey);
  addObj(wKey);
  addObj(eKey);
  addObj(rKey);
  addObj(tKey);
  addObj(yKey);
  addObj(uKey);
  addObj(iKey);
  addObj(oKey);
  addObj(pKey);

  bmpInputKey* aKey = new bmpInputKey("a", "-", "", col(1,2), row(2), keyWidth, keyHeight, this);
  bmpInputKey* sKey = new bmpInputKey("s", "/", "", col(2,2), row(2), keyWidth, keyHeight, this);
  bmpInputKey* dKey = new bmpInputKey("d", ":", "", col(3,2), row(2), keyWidth, keyHeight, this);
  bmpInputKey* fKey = new bmpInputKey("f", ";", "", col(4,2), row(2), keyWidth, keyHeight, this);
  bmpInputKey* gKey = new bmpInputKey("g", "(", "", col(5,2), row(2), keyWidth, keyHeight, this);
  bmpInputKey* hKey = new bmpInputKey("h", ")", "", col(6,2), row(2), keyWidth, keyHeight, this);
  bmpInputKey* jKey = new bmpInputKey("j", "$", "", col(7,2), row(2), keyWidth, keyHeight, this);
  bmpInputKey* kKey = new bmpInputKey("k", "@", "", col(8,2), row(2), keyWidth, keyHeight, this);
  bmpInputKey* lKey = new bmpInputKey("l", "\"", "", col(9,2), row(2), keyWidth, keyHeight, this);

  addObj(aKey);
  addObj(sKey);
  addObj(dKey);
  addObj(fKey);
  addObj(gKey);
  addObj(hKey);
  addObj(jKey);
  addObj(kKey);
  addObj(lKey);

  bmpInputKey* zKey = new bmpInputKey("z", ".", "", col(2,3), row(3), keyWidth, keyHeight, this);
  bmpInputKey* xKey = new bmpInputKey("x", ",", "", col(3,3), row(3), keyWidth, keyHeight, this);
  bmpInputKey* cKey = new bmpInputKey("c", "?", "", col(4,3), row(3), keyWidth, keyHeight, this);
  bmpInputKey* vKey = new bmpInputKey("v", "!", "", col(5,3), row(3), keyWidth, keyHeight, this);
  bmpInputKey* bKey = new bmpInputKey("b", "&", "", col(6,3), row(3), keyWidth, keyHeight, this);
  bmpInputKey* nKey = new bmpInputKey("n", "%", "", col(7,3), row(3), keyWidth, keyHeight, this);
  bmpInputKey* mKey = new bmpInputKey("m", "#", "", col(8,3), row(3), keyWidth, keyHeight, this);

  addObj(zKey);
  addObj(xKey);
  addObj(cKey);
  addObj(vKey);
  addObj(bKey);
  addObj(nKey);
  addObj(mKey);

  bmpControlKey* shiftKey = new bmpControlKey("^", shift, col(1,3), row(3), keyWidth*1.5, keyHeight, this, getBmpPath(shiftBmp));
  addObj(shiftKey);
  
  bmpControlKey* backSpKey = new bmpControlKey("<", backspace, col(9,3), row(3), keyWidth*1.5, keyHeight, this, getBmpPath(deleteBmp));
  addObj(backSpKey);
  
  if (modal) {
    bmpInputKey* spcKey  = new bmpInputKey(" ", " ", " ", col(5,4), row(4), (keyWidth*3), keyHeight, this);
    addObj(spcKey);
    bmpControlKey* leftArrow  = new bmpControlKey("<", arrowBack, col(4,4), row(4), keyWidth, keyHeight, this, getBmpPath(lArrowBmp));
    addObj(leftArrow);
    bmpControlKey* rightArrow  = new bmpControlKey(">", arrowFWD, col(8,4), row(4), keyWidth, keyHeight, this, getBmpPath(rArrowBmp));
    addObj(rightArrow);
    bmpControlKey* symbolKey   = new bmpControlKey("#", number, col(3,4), row(4), keyWidth, keyHeight, this, getBmpPath(symbolBmp));
    addObj(symbolKey);
    bmpControlKey* cancelKey   = new bmpControlKey("x", cancel, col(1,4), row(4), keyWidth*1.5, keyHeight, this, getBmpPath(redXBmp));
    addObj(cancelKey);
    bmpControlKey* oKKey  = new bmpControlKey(">", ok, col(9,4), row(4), keyWidth*1.5, keyHeight, this, getBmpPath(checkBmp));
    addObj(oKKey);
  } else {
    bmpInputKey* spcKey  = new bmpInputKey(" ", " ", " ", col(4,4), row(4), (keyWidth*3), keyHeight, this);
    addObj(spcKey);
    bmpControlKey* leftArrow  = new bmpControlKey("<", arrowBack, col(3,4), row(4), keyWidth, keyHeight, this, getBmpPath(lArrowBmp));
    addObj(leftArrow);
    bmpControlKey* rightArrow  = new bmpControlKey(">", arrowFWD, col(7,4), row(4), keyWidth, keyHeight, this, getBmpPath(rArrowBmp));
    addObj(rightArrow);
    bmpControlKey* symbolKey   = new bmpControlKey("#", number, col(1,4), row(4), keyWidth*1.5, keyHeight, this, getBmpPath(symbolBmp));
    addObj(symbolKey);
    bmpControlKey* enterKey  = new bmpControlKey(">", enter, col(9,4), row(4), keyWidth*2, keyHeight, this, getBmpPath(returnBmp));
    addObj(enterKey);
  }
}


// *****************************************************
//                       bmpInputKey
// *****************************************************



bmpInputKey::bmpInputKey(const char* inLabel, const char* inNum, const char* inSym, int inX, int inY, int inWidth, int inHeight, bmpKeyboard* inKeyboard)
: inputKey(inLabel, inNum, inSym, inX, inY, inWidth, inHeight, inKeyboard) {

	ourKeyboard = inKeyboard;						// Need a pointer to this for path names. (Poor design?)
	keyBMap = inKeyboard->getKeyMap();			// The keyboard has the bitmap background.
	setColors(inKeyboard->getKeyTextColor());	// And our favorite text color..
	setTextSize(2);
}


bmpInputKey::~bmpInputKey(void) { }


void bmpInputKey::drawSelf(void) {
	
	bitmap		ourBMap(keyBMap); // Ah! we make a copy of the background.
	rect			sRect;
	offscreen	vPort; 
	colorObj		aPixel;
	point			center;
	point			current;
	float			dist;
	mapper		distMapper(4,12,100,0);
		
	if (buff[0]==' ') {               // Special hack for spacebar.
		if (clicked) {
			screen->fillRect(this, &white);
		} else {
			bmpObj aBmp(x,y,width,height,ourKeyboard->getBmpPath(ourKeyboard->spaceBmp));
			aBmp.drawSelf();
		}
	} else {                          // "Normal printing
		center.x = width/2;
		center.y = height/2;
		vPort.beginDraw(&ourBMap,x,y);	// We start drawing to it, it already has background image.
		x = x + 7;
		y = y + 9;
		label::drawSelf();
		x = x - 7;
		y = y - 9;
		if (clicked) {
			for(int i=0;i<width;i++) {
				for(int j=0;j<height;j++) {
					aPixel = ourBMap.getColor(i,j);
					current.x = i;
					current.y = j;
					dist = distance(center,current);
					aPixel.blend(&white,distMapper.map(dist));
					ourBMap.setColor(i,j,&aPixel);
				}
			}
		}
		vPort.endDraw();
		screen->blit(x,y,&ourBMap);
	}
}


// *****************************************************
//                       bmpControlKey
// *****************************************************



bmpControlKey::bmpControlKey(const char* inLabel, keyCommands inCom, int inX, int inY, int inWidth, int inHeight, bmpKeyboard* inKeyboard, const char* bmpName)
	: controlKey(inLabel, inCom, inX, inY, inWidth, inHeight, inKeyboard) { ourBMPObj = new bmpObj(inX,inY,inWidth,inHeight,bmpName); }


  bmpControlKey::~bmpControlKey(void) { if (ourBMPObj) delete(ourBMPObj); }


  void bmpControlKey::drawSelf(void) {
    
    if (clicked) {
      screen->fillRect(this, &white);
    } else if (ourBMPObj) {
      	ourBMPObj->setLocation(x,y);	// Just in case we moved. Remember this was just hacked on.
      	ourBMPObj->drawSelf();
    } else {
    	screen->fillRect(this, &red);
    }
  }
