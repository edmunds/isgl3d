/*
 * iSGL3D: http://isgl3d.com
 *
 * Copyright (c) 2010-2012 Stuart Caunt
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#import "Isgl3dScene3D.h"
#import "Isgl3dNode.h"
#import "Isgl3dNodeDynamics.h"
#import "Isgl3dScenePhysics.h"

@interface Isgl3dScene3D()
@property (nonatomic, readwrite, retain) NSMutableDictionary *nodesByName;
@property (nonatomic, readwrite, retain) NSMutableSet *cameras;
@end

@implementation Isgl3dScene3D
@synthesize nodesByName = _nodesByName;
@synthesize cameras = _cameras;

+ (id)scene {
	return [[[self alloc] init] autorelease];
}

- (id)init {
    if ((self = [super init])) {
		_alphaNodes = [[NSMutableArray alloc] init];
		_sortedNodes = [[NSMutableArray alloc] init];
        _nodesByName = [[NSMutableDictionary alloc] init];
        _cameras = [[NSMutableSet alloc] init];
    }
	
    return self;
}

- (void)dealloc {
	[_sortedNodes release];
	[_alphaNodes release];
    [_nodesByName release];
    [_cameras release];
    [_physics release];
	[super dealloc];
}

- (void)renderZSortedAlphaObjects:(Isgl3dGLRenderer *)renderer viewMatrix:(Isgl3dMatrix4 *)viewMatrix {

	// Collect in array all transparent objects in the scene
	[self collectAlphaObjects:_alphaNodes];
	
	//Isgl3dLog(Info, @"Number of alphas = %i", [_alphaNodes count]);
	
	// Create a sortable nodes for each of them, with the distance of the object origin to the camera
	for (Isgl3dNode * node in _alphaNodes) {
		float distance = [node getZTransformation:viewMatrix];
		Isgl3dSortableNode * sortableNode = [[Isgl3dSortableNode alloc] initWithDistance:distance forNode:node];
		[_sortedNodes addObject:[sortableNode autorelease]];
	}
	
	// Sort the objects, DESCENGING order by distance
	[_sortedNodes sortUsingSelector:@selector(compareDistances:)];
	
	// Render each node, release SortableNode afterwards
	for (Isgl3dSortableNode * sortableNode in _sortedNodes) {
		[sortableNode.node render:renderer opaque:FALSE];
	}
	
	// Clear arrays
	[_alphaNodes removeAllObjects];
	[_sortedNodes removeAllObjects];
}

- (void)setGravity:(Isgl3dVector3)gravity {
    if (!_physics) {
        self.physics = [Isgl3dScenePhysics new];
    }
    _physics.gravity = gravity;
}

- (Isgl3dVector3)gravity {
    if (!_physics) {
        self.physics = [Isgl3dScenePhysics new];
    }
    return _physics ? _physics.gravity : Isgl3dVector3Make(0, 0, 0);
}

- (void)descendantAdded:(Isgl3dNode *)descendant {
    if (descendant.dynamics && _physics) {
        [_physics addNodeDynamics:descendant.dynamics];
    }
    NSString *name = descendant.name;
    if (name) {
        _nodesByName[name] = descendant;
    }
}

- (void)descendantRemoved:(Isgl3dNode *)descendant {
    if (descendant.dynamics && _physics) {
        [_physics removeNodeDynamics:descendant.dynamics];
    }
    NSString *name = descendant.name;
    if (name) { //TODO: indexing could be a performance problem with large graphs
        [_nodesByName removeObjectForKey:name];
    }
}

- (void)descendantDidAddDynamics:(Isgl3dNode *)descendant {
    if (descendant.dynamics && _physics) {
        [_physics addNodeDynamics:descendant.dynamics];
    }
}

- (void)descendantWillRemoveDynamics:(Isgl3dNode *)descendant {
    if (descendant.dynamics && _physics) {
        [_physics removeNodeDynamics:descendant.dynamics];
    }
}

- (void)updateWorldTransformation:(GLKMatrix4 *)parentTransformation {
    if (_physics) {
        [_physics updateDynamics];
    }
    [super updateWorldTransformation:parentTransformation];
}


@end


#pragma mark Isgl3dSortableNode

@implementation Isgl3dSortableNode

@synthesize distance = _distance;
@synthesize node = _node;

- (id)initWithDistance:(float)distance forNode:(Isgl3dNode *)node {
    if ((self = [super init])) {
    	_distance = distance;
    	
    	_node = [node retain];
    }
	
    return self;
}

- (void)dealloc {
	[_node release];

	[super dealloc];
}

- (NSComparisonResult) compareDistances:(Isgl3dSortableNode *)node {
	NSComparisonResult retVal = NSOrderedSame;
	if (_distance < node.distance) {
		retVal = NSOrderedAscending;
	} else if (_distance > node.distance) { 
		retVal = NSOrderedDescending;
	}
	return retVal;
}

@end
