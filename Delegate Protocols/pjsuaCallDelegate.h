//
//  pjsuaCallDelegate.h
//  Reception
//
//  Created by Sye Boddeus on 24/03/2014.
//  Copyright (c) 2014 UOW. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol pjsuaCallDelegate <NSObject>

- (void)pjsuaCallEnded;

- (void)pjsuaCallBegan;
- (void)pjsuaCallFailed;
- (void)pjsuaCallRejected;

@end
