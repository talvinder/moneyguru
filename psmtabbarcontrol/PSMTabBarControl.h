//
//  PSMTabBarControl.h
//  PSMTabBarControl
//
//  Created by John Pannell on 10/13/05.
//  Copyright 2005 Positive Spin Media. All rights reserved.
//  Copyright 2011 Hardcoded Software (http://www.hardcoded.net)
//

/*
 This view provides a control interface to manage a regular NSTabView.  It looks and works like the tabbed browsing interface of many popular browsers.
 */

#import <Cocoa/Cocoa.h>

#define kPSMTabBarControlHeight 22
// internal cell border
#define MARGIN_X        6
#define MARGIN_Y        3
// padding between objects
#define kPSMTabBarCellPadding 4
// fixed size objects
#define kPSMMinimumTitleWidth 30
#define kPSMTabBarIndicatorWidth 16.0
#define kPSMTabBarIconWidth 16.0
#define kPSMHideAnimationSteps 2.0

@class PSMOverflowPopUpButton;
@class PSMRolloverButton;
@class PSMTabBarCell;
@protocol PSMTabStyle;

enum {
    PSMTab_SelectedMask                 = 1 << 1,
    PSMTab_LeftIsSelectedMask		= 1 << 2,
    PSMTab_RightIsSelectedMask          = 1 << 3,
    PSMTab_PositionLeftMask		= 1 << 4,
    PSMTab_PositionMiddleMask		= 1 << 5,
    PSMTab_PositionRightMask		= 1 << 6,
    PSMTab_PositionSingleMask		= 1 << 7
};

@interface PSMTabBarControl : NSControl <NSTabViewDelegate, NSDraggingSource>
{
    
    // control basics
    NSMutableArray              *_cells;                    // the cells that draw the tabs
    IBOutlet NSTabView          *tabView;                   // the tab view being navigated
    PSMOverflowPopUpButton      *_overflowPopUpButton;      // for too many tabs
    PSMRolloverButton           *_addTabButton;
    
    // drawing style
    id<PSMTabStyle>             style;
    BOOL                        _canCloseOnlyTab;
    BOOL                        _hideForSingleTab;
    BOOL                        _showAddTabButton;
    BOOL                        _sizeCellsToFit;
    
    // cell width
    NSInteger                   _cellMinWidth;
    NSInteger                   _cellMaxWidth;
    NSInteger                   _cellOptimumWidth;
    
    // animation for hide/show
    NSInteger                   _currentStep;
    BOOL                        _isHidden;
    BOOL                        _hideIndicators;
    IBOutlet id                 partnerView;                // gets resized when hide/show
    BOOL                        _awakenedFromNib;
    
    // drag and drop
    NSEvent                     *_lastMouseDownEvent;      // keep this for dragging reference   
    BOOL			_allowsDragBetweenWindows;
    
    // MVC help
    IBOutlet id                 delegate;
}

// control characteristics
+ (NSBundle *)bundle;

// control configuration
- (BOOL)canCloseOnlyTab;
- (void)setCanCloseOnlyTab:(BOOL)value;
- (id<PSMTabStyle>)style;
- (NSString *)styleName;
- (BOOL)hideForSingleTab;
- (void)setHideForSingleTab:(BOOL)value;
- (BOOL)showAddTabButton;
- (void)setShowAddTabButton:(BOOL)value;
- (NSInteger)cellMinWidth;
- (void)setCellMinWidth:(NSInteger)value;
- (NSInteger)cellMaxWidth;
- (void)setCellMaxWidth:(NSInteger)value;
- (NSInteger)cellOptimumWidth;
- (void)setCellOptimumWidth:(NSInteger)value;
- (BOOL)sizeCellsToFit;
- (void)setSizeCellsToFit:(BOOL)value;
- (BOOL)allowsDragBetweenWindows;
- (void)setAllowsDragBetweenWindows:(BOOL)flag;

// accessors
- (NSTabView *)tabView;
- (void)setTabView:(NSTabView *)view;
- (id)delegate;
- (void)setDelegate:(id)object;
- (id)partnerView;
- (void)setPartnerView:(id)view;
- (PSMTabBarCell *)cellAtIndex:(NSInteger)index;
- (PSMTabBarCell *)cellForTab:(NSTabViewItem *)tabItem;

// the buttons
- (PSMRolloverButton *)addTabButton;
- (PSMOverflowPopUpButton *)overflowPopUpButton;
- (NSMutableArray *)representedTabViewItems;

// utilities
- (void)hideTabBar:(BOOL)hide animate:(BOOL)animate;
- (void)insertCell:(PSMTabBarCell *)aCell fromTabBar:(PSMTabBarControl *)aSourceBar beforeCell:(PSMTabBarCell *)aDropCell;

@end


@interface NSObject (TabBarControlDelegateMethods)
- (BOOL)tabView:(NSTabView *)aTabView shouldCloseTabViewItem:(NSTabViewItem *)aTabViewItem;
- (void)tabView:(NSTabView *)aTabView willCloseTabViewItem:(NSTabViewItem *)aTabViewItem;
- (void)tabView:(NSTabView *)aTabView didCloseTabViewItem:(NSTabViewItem *)aTabViewItem;
- (void)tabView:(NSTabView *)aTabView movedTab:(NSTabViewItem *)aTabViewItem fromIndex:(NSInteger)aFrom toIndex:(NSInteger)aTo;
@end