#include <IOandKeys.h>
#include <resizeBuff.h>


IOandKeys::IOandKeys(editLabel* inEditField,textView* inTextField) 
	: keyboard(inEditField) {
    
	mEditField = inEditField;
	if (mEditField) {
		mEditField->setEventSet(touchLift);
		mEditField->setExitOnEnter(true);
		mEditField->beginEditing();
	}
	mTextField = inTextField;
	mHaveBuff = false;
	mOutBuff = NULL;
	mNumBytes = 0;
}         


IOandKeys::~IOandKeys(void) {  }


void IOandKeys::handleKey(keyCommands inEditCom) {
  
	keyboard::handleKey(inEditCom);						// He's just running along..
	if (inEditCom == enter) {								// Got the return key.
		mNumBytes = mEditField->getNumChars();			// How big is this new message?
		mNumBytes++;												// Add one for NULL.
		if (resizeBuff(mNumBytes,&mOutBuff)) {			// If we got the room..
			mEditField->getText(mOutBuff);				// Grab the message from the edit field.
			mTextField->appendText(mOutBuff);			// Echo it.
			mTextField->appendText("\n");					// Line feed.
			mHaveBuff = true;									// Yes, we have text.
		}															//
		mEditField->setValue("");							// Clear the editField.
		mEditField->setIndex(0);							// Cursor goes home.
		mEditField->beginEditing();						// Restart the editing.
	}
}


int IOandKeys::haveBuff(void) {

	if (mHaveBuff) {
		return mNumBytes;
	}
	return 0;
}


bool IOandKeys::getBuff(char* theirBuff,int maxBytes) {
	
	for (int i=0;i<mNumBytes;i++) {
		if (i<maxBytes) {
			theirBuff[i] = mOutBuff[i];
		}
	}
	resizeBuff(0,&mOutBuff);
	mHaveBuff = false;
	return true;
}


/*
bool IOandKeys::resizeOutBuff(byte numBytes) {

  mNumBytes = 0;
  if (mOutBuff) {
    free(mOutBuff);
    mOutBuff = NULL;
  }
  if (numBytes>0) {
    mOutBuff = (char*)malloc(numBytes);
    if (mOutBuff) {
      mNumBytes = numBytes;
      return true;
    } else {
      return false;
    }
  }
  return true;              // Because they asked for none.
}
*/