//
//  RCPCall.h
//  Reception
//
//  Created by Sye Boddeus on 24/03/2014.
//  Copyright (c) 2014 UOW. All rights reserved.
//

#ifndef Reception_RCPCall_h
#define Reception_RCPCall_h

#ifdef __cplusplus
#include <iostream>
#endif

#include <pjsua2.hpp>

#import "pjsuaCallDelegate.h"

typedef id<pjsuaCallDelegate> callDelegate;

using namespace pj;

class RCPCall : public Call {
private:
    callDelegate mAgent;
    
public:
    RCPCall(Account &acc, int callID = PJSUA_INVALID_ID) : Call(acc, callID)
    {};
    
    void setCallBackAgent(callDelegate agent) {
        mAgent = agent;
    }
    
    void onCallState(OnCallStateParam &prm) {
        std::cout << prm.e.type << std::endl;
        if (prm.e.type == PJSIP_EVENT_TSX_STATE) {
            if (prm.e.body.tsxState.tsx.method == "BYE") {
                [mAgent pjsuaCallEnded];
            }
            else if (prm.e.body.tsxState.tsx.statusText == "Busy Here") {
                [mAgent pjsuaCallRejected];
            }
            else {
                std::cout << "TSX MESSAGE: " << prm.e.body.tsxState.tsx.method << std::endl;
                std::cout << "TSX MESSAGE EXTENDED: " << prm.e.body.tsxState.tsx.statusText << std::endl;
            }
        }
        else if (prm.e.type == PJSIP_EVENT_TRANSPORT_ERROR) {
            [mAgent pjsuaCallFailed];
        }
        else {
            std::cout << prm.e.type << " MEDIA TYPE RECIEVED" << std::endl;
        }
    }
    
    void onCallMediaState(OnCallMediaStateParam &prm) {
        CallInfo ci = getInfo();
        // Iterate all the call medias
        for (unsigned i = 0; i < ci.media.size(); i++) {
            if (ci.media[i].type==PJMEDIA_TYPE_AUDIO && getMedia(i)) {
                AudioMedia *aud_med = (AudioMedia *)getMedia(i);
                
                // Connect the call audio media to sound device
                AudDevManager& mgr = Endpoint::instance().audDevManager();
                aud_med->startTransmit(mgr.getPlaybackDevMedia());
                mgr.getCaptureDevMedia().startTransmit(*aud_med);
            }
        }
        NSLog(@"ADDDED AUDIO DEVICES");
    }
};

#endif
