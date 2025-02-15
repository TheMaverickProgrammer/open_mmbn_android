#include "bnAnimate.h"

#include <iostream>

Animate::Animate() {
  onFinish = nullptr;
  queuedOnFinish = nullptr;
  isUpdating = false;
  callbacksAreValid = true;
}

Animate::Animate(Animate& rhs) {
  this->onFinish = rhs.onFinish;
  this->callbacks = rhs.callbacks;
  this->onetimeCallbacks = rhs.onetimeCallbacks;
  this->nextLoopCallbacks = rhs.nextLoopCallbacks;
  this->queuedCallbacks = rhs.queuedCallbacks;
  this->queuedOnetimeCallbacks = rhs.queuedOnetimeCallbacks;
  this->queuedOnFinish = rhs.queuedOnFinish;
  this->isUpdating = rhs.isUpdating;
  this->callbacksAreValid = rhs.callbacksAreValid;
}

Animate::~Animate() {
  this->callbacks.clear();
  this->queuedCallbacks.clear();
  this->onetimeCallbacks.clear();
  this->queuedOnetimeCallbacks.clear();
  this->nextLoopCallbacks.clear();
  this->onFinish = nullptr;
  this->queuedOnFinish = nullptr;
}

void Animate::operator() (float progress, sf::Sprite& target, FrameList& sequence)
{
  // std::cout << "progress: " << progress << ", frames: " << sequence.frames.size() << ", totalDur: " << sequence.totalDuration << std::endl;
  float startProgress = progress;
  
  // Callbacks are only invalide during clears in the update loop
  if(!callbacksAreValid) callbacksAreValid = true;
  
  isUpdating = true;
  // std::cout << "callbacks size: " << this->callbacks.size() << " for " << sequence.frames.size() << " frames " << std::endl;

  if (sequence.frames.empty()) {
    if (onFinish != nullptr) {
      onFinish();
      onFinish = nullptr;
    }
    
    isUpdating = false;
    callbacksAreValid = true;
    callbacks.insert(queuedCallbacks.begin(), queuedCallbacks.end());
    queuedCallbacks.clear();
    
    onetimeCallbacks.insert(queuedOnetimeCallbacks.begin(), queuedOnetimeCallbacks.end());
    queuedOnetimeCallbacks.clear();
    
    if(queuedOnFinish) {
		onFinish = queuedOnFinish;
		queuedOnFinish = nullptr;
	}
	
    return;
  }

  bool applyCallback = (sequence.totalDuration == 0 || (startProgress > sequence.totalDuration && startProgress > 0.f));

  if (applyCallback) {
    if (onFinish != nullptr) {
      onFinish();
      
      if((playbackMode & Mode::Loop) != Mode::Loop) {
		 onFinish = nullptr;
	  }
	  
	  isUpdating = false; 
	  callbacksAreValid = true;
	  callbacks.insert(queuedCallbacks.begin(), queuedCallbacks.end());
      queuedCallbacks.clear();
      
      onetimeCallbacks.insert(queuedOnetimeCallbacks.begin(), queuedOnetimeCallbacks.end());
      queuedOnetimeCallbacks.clear();
      
      if(queuedOnFinish) {
        onFinish = queuedOnFinish;
        queuedOnFinish = nullptr;
	  }
	  
      return;
    }
  }

  std::vector<Frame> copy = sequence.frames;

  if ((playbackMode & Mode::Reverse) == Mode::Reverse) {
    reverse(copy.begin(), copy.end());
  }

  int index = 0;
  std::vector<Frame>::const_iterator iter = copy.begin();


  while (iter != copy.end() && startProgress != 0.f) {
    index++;
    progress -= (*iter).duration;

    // Must be <= and not <, to handle case (progress == frame.duration) correctly
    // We assume progress hits zero because we use it as a decrementing counter
    // We add a check to ensure the start progress wasn't also 0
    // If it did not start at zero, we know we came across the end of the animation
    if ((progress <= 0.f || &(*iter) == &copy.back()) && startProgress != 0.f)
    {
      std::map<int, std::function<void()>>::iterator callbackIter, callbackFind = this->callbacks.find(index - 1);
      std::map<int, std::function<void()>>::iterator onetimeCallbackIter = this->onetimeCallbacks.find(index - 1);

	  callbackIter = callbacks.begin();
	  
      while (callbacksAreValid && callbackIter != callbackFind && callbackFind != this->callbacks.end()) {
        if(callbackIter->second) {
		  //std::cout << "functor callback invoked" << std::endl;
          callbackIter->second();
          //std::cout << "post functor callback invocation" << std::endl;
	    } else {
		  //std::cout << "callback iter functor was null" << std::endl;
		}
		
		if(!callbacksAreValid) break;
        
        nextLoopCallbacks.insert(*callbackIter);
        callbackIter = callbacks.erase(callbackIter);
        callbackFind = callbacks.find(index - 1);
      }
      
      if(callbacksAreValid && callbackIter == callbackFind && callbackFind != this->callbacks.end()) 
      {
        if(callbackIter->second) {
          callbackIter->second();
	    }
        
        if(callbacksAreValid) {
          nextLoopCallbacks.insert(*callbackIter);
          callbackIter = callbacks.erase(callbackIter);	  
	    } 
	  }

      if (callbacksAreValid && onetimeCallbackIter != this->onetimeCallbacks.end()) {
        if(onetimeCallbackIter->second) {
          onetimeCallbackIter->second();
	    }
	    
	    if(callbacksAreValid) {
	      onetimeCallbacks.erase(onetimeCallbackIter);
	    }
      }

      if ((playbackMode & Mode::Loop) == Mode::Loop && progress > 0.f && &(*iter) == &copy.back()) {
        if ((playbackMode & Mode::Bounce) == Mode::Bounce) {
          reverse(copy.begin(), copy.end());
          iter = copy.begin();
          iter++;
        }
        else {
          iter = copy.begin();
        }

		this->callbacks.clear();
        this->callbacks = nextLoopCallbacks;
        this->nextLoopCallbacks.clear();
        
        callbacksAreValid = true;

        continue; // Start loop again
      }

	  target.setTextureRect((*iter).subregion);
	  if ((*iter).applyOrigin) {
		target.setOrigin((float)(*iter).origin.x, (float)(*iter).origin.y);
	  }
	  
      break;
    }

    iter++;
  }
  
  if(iter != copy.end()) {
    target.setTextureRect((*iter).subregion);
    if ((*iter).applyOrigin) {
	  target.setOrigin((float)(*iter).origin.x, (float)(*iter).origin.y);
    }
  }
  
  isUpdating = false;
  callbacksAreValid = true;
  //std::cout << "callbacks size before merge with queued: " << callbacks.size() << std::endl;
  //std::cout << "queued size: " << queuedCallbacks.size() << std::endl;

  callbacks.insert(queuedCallbacks.begin(), queuedCallbacks.end());
  queuedCallbacks.clear();
  
  onetimeCallbacks.insert(queuedOnetimeCallbacks.begin(), queuedOnetimeCallbacks.end());
  queuedOnetimeCallbacks.clear();
  
  if(queuedOnFinish) {
    onFinish = queuedOnFinish;
    queuedOnFinish = nullptr;
  }

  //std::cout << "callbacks size after merge with queued: " << callbacks.size() << std::endl;
}

Animate & Animate::operator<<(On rhs)
{
  if(!rhs.callback) return *this;
  
  if (rhs.doOnce) {
	  if(this->isUpdating) {
		  this->queuedOnetimeCallbacks.insert(std::make_pair(rhs.id, rhs.callback));
	  } else {
          this->onetimeCallbacks.insert(std::make_pair(rhs.id, rhs.callback));
      }
  }
  else {
	if(this->isUpdating) {
		// std:: cout << "callback queued" << std::endl;
        this->queuedCallbacks.insert(std::make_pair(rhs.id, rhs.callback));
    } else {
        this->callbacks.insert(std::make_pair(rhs.id, rhs.callback));		
	}
  }

  return *this;
}

Animate & Animate::operator<<(char rhs)
{
  this->playbackMode = rhs;
  return *this;
}

void Animate::operator<<(std::function<void()> finishNotifier)
{
  if(!finishNotifier) return;
  
  if(!this->isUpdating) {
    this->onFinish = finishNotifier;
  } else {
	this->queuedOnFinish = finishNotifier;
  }
}

void Animate::SetFrame(int frameIndex, sf::Sprite & target, FrameList& sequence) const
{
  int index = 0;
  for (Frame& frame : sequence.frames) {
    index++;

    if (index == frameIndex) {
      target.setTextureRect(frame.subregion);
      if (frame.applyOrigin) {
        target.setOrigin((float)frame.origin.x, (float)frame.origin.y);
      }

      return;
    }
  }
}
