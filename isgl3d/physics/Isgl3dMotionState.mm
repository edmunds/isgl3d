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

#import "Isgl3dMotionState.h"
#import "Isgl3dNode.h"

Isgl3dMotionState::Isgl3dMotionState(Isgl3dNode * node) :
	_node(node) {
}

Isgl3dMotionState::~Isgl3dMotionState() {
}

void Isgl3dMotionState::getWorldTransform(btTransform& centerOfMassWorldTrans) const {
	centerOfMassWorldTrans.setFromOpenGLMatrix(_node.bulletTransform.m);
}

void Isgl3dMotionState::setWorldTransform(const btTransform& centerOfMassWorldTrans) {
    Isgl3dMatrix4 transform =  GLKMatrix4Identity;
	centerOfMassWorldTrans.getOpenGLMatrix(transform.m);
	_node.bulletTransform = transform;
}