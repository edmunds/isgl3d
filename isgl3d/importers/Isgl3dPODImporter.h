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

#import <Foundation/Foundation.h>

@class Isgl3dGLMesh;
@class Isgl3dMaterial;
@class Isgl3dNode;
@class Isgl3dMeshNode;
@class Isgl3dSkeletonNode;
@class Isgl3dNodeCamera;
@class Isgl3dLight;

/**
 * The Isgl3dPODImporter provides an example of how to obtain data from a POWERVR POD
 * scene data file and convert the data into iSGL3D objects.
 *
 * Essentially, the Isgl3dPODImporter provides an interface to the POWERVR POD importer tools available from
 * the Imagination Technologies Limited, POWERVR SDK (http://www.imgtec.com/powervr/insider/powervr-sdk.asp),
 * of which some of the C++ tools classes are contained here in the pvrtools directory with kind permission
 * of Imagination Technologies Limited.
 *
 * With this importer, full scene details can be retrieved and reproduced in iSGL3D including:
 * <ul>
 * <li>Simple non-animated meshes</li>
 * <li>Animated (skinned) meshes</li>
 * <li>Bones forming a skeleton</li>
 * <li>Texture and color materials</li>
 * <li>Lights</li>
 * <li>Cameras</li>
 * </ul>
 *
 * For meshes and materials, skinning data is obtained from the pod as are bones. Both animated meshes and
 * bones (or rather joints) can be rendered.
 *
 * Texture names contained in the POD data can be overridden to use user-defined textures.
 *
 * Note, Isgl3dPODImporter.h is not included in the main isgl3d.h file to avoid compilation errors arising
 * from the use of C++ classes.
 *
 */
@interface Isgl3dPODImporter : NSObject
/**
 * Allocates and initialises (autorelease) an importer with the POD data file from the main bundle resources.
 * @param path The name of the resource file.
 */
+ (id)podImporterWithResource:(NSString *)name;

/**
 * Allocates and initialises (autorelease) an importer with the POD data file from the main bundle resources.
 * @param path The name of the resource file.
 * @param highlightParents Highlights parent nodes with a yellow bone mesh sphere for debugging.
 * @param flipTextures flip all POD textures.
 */
+ (id)podImporterWithResource:(NSString *)name highlightParents:(BOOL)highlightParents flipTextures:(BOOL)flipTextures;

/**
 * Allocates and initialises (autorelease) an importer with the POD data file path.
 * @param path The path to the POD data file.
 */
+ (id)podImporterWithFile:(NSString *)filePath;

/**
 * Initialises the importer with the POD data file from the main bundle resources.
 * @param path The name of the resource file.
 */
- (id)initWithResource:(NSString *)name;

/**
 * Initialises the importer with the POD data file path.
 * @param path The path to the POD data file.
 */
- (id)initWithFile:(NSString *)filePath;

/**
 * Prints to the console information about the structure and contents of the POD.
 */
- (void)printPODInfo;
/**
 * Returns the number of mesh nodes in the scene.
 * @return The number of mesh nodes in the scene.
 */
@property (readonly) unsigned int numberOfMeshNodes;

/**
 * Returns the number of nodes in the scene.
 * @return The number of nodes in the scene.
 */
@property (readonly) unsigned int numberOfNodes;

/**
 * Returns the number of meshes in the scene.
 * @return The number of meshes in the scene.
 */
@property (readonly) unsigned int numberOfMeshes;

/**
 * Returns the number of cameras in the scene.
 * @return The number of cameras in the scene.
 */
@property (readonly) unsigned int numberOfCameras;

/**
 * Returns the number of lights in the scene.
 * @return The number of lights in the scene.
 */
@property (readonly) unsigned int numberOfLights;

/**
 * Returns the number of frames in the scene (for the bone/mesh animation).
 * @return The number of frames in the scene (for the bone/mesh animation).
 */
@property (readonly) unsigned int numberOfFrames;
/**
 * Returns the scene ambient color (rgb) as defined in the POD file.
 * @return the scene ambient color (rgb) as defined in the POD file.
 */
@property (readonly) float *ambientColor;
/**
 * Returns the scene background color (rgb) as defined in the POD file.
 * @return the scene background color (rgb) as defined in the POD file.
 */
@property (readonly) float *backgroundColor;
/**
 * Returns the materials in the pod file
 * @return the materials in the pod file
 */
@property (nonatomic, retain, readonly) NSArray *materials;

/**
 * Builds all the scene objects in the POD file without adding them to the scene. The POD objects can
 * then be used afterwards by retreiving them with the necessary accessors.
 * Note: A call to this method is not required if using addMeshesToScene.
 */
- (void)buildSceneObjects;

/**
 * Adds Isgl3dMeshNodes containing the meshes and relevant materaisl to the scene (or any other Isgl3dNode)
 * from the data in the POD file.
 * This creates a number of Isgl3dMeshNodes taking both material and mesh data, along with
 * transformations for the node provided by the file. All animated meshes are also added to the scene.
 * @param scene The node to which the POD scene contents are added to as children.
 */
- (void)addMeshesToScene:(Isgl3dNode *)scene;
/**
 * Adds all nodes, includes meshes, bones, cameras, and lights.
 * @param scene The node to which the POD scene contents are added to as children.
 */
- (void)addNodesToScene:(Isgl3dNode *)scene;
/**
 * Adds Isgl3dBoneNodes to an Isgl3dSkeletonNode from the data in the POD file.
 * The bone nodes contain the animation transformations used for the skinning but this is not necessary
 * to produce animated meshes (this is done by the addMeshesToScene: method). Adding the bones can
 * be useful to view the movement of the bones without a mesh.
 * @param skeleton The Isgl3dSkeletonNode to which the bones are added.
 */
- (void)addBonesToSkeleton:(Isgl3dSkeletonNode *)skeleton;

/**
 * Returns the Isgl3dGLMesh corresponding to a mesh node name (as defined in the POD file).
 * All mesh names can be obtained via printPODInfo.
 * @param meshName the name of the mesh as defined in the POD file.
 * @return The Corresponding Isgl3dGLMesh.
 */
- (Isgl3dGLMesh *) meshFromNodeWithName:(NSString *)meshName;

/**
 * Returns the Isgl3dGLMesh corresponding to a mesh index (as defined in the POD file).
 * All mesh indices can be obtained via printPODInfo.
 * @param meshIndex the index of the mesh as defined in the POD file.
 * @return The Corresponding Isgl3dGLMesh.
 */
- (Isgl3dGLMesh *) meshAtIndex:(unsigned int)meshIndex;

/**
 * Returns the Isgl3dMeshNode corresponding to a node name (as defined in the POD file).
 * All node names can be obtained via printPODInfo.
 * @param nodeName the name of the node as defined in the POD file.
 * @return The Corresponding Isgl3dMeshNode.
 */
- (Isgl3dMeshNode *) meshNodeWithName:(NSString *)nodeName;

/**
 * Returns the node (mesh or generic structural node) corresponding to a node name (as
 * defined in the POD file).
 * All node names can be obtained via printPODInfo.
 * @param nodeName the name of the node as defined in the POD file.
 * @return The Corresponding Isgl3dMeshNode.
 */
- (Isgl3dNode *) nodeWithName:(NSString *)nodeName;

/**
 * Returns the Isgl3dMaterial corresponding to a material name (as defined in the POD file).
 * All material names can be obtained via printPODInfo.
 * @param materialName the name of the material as defined in the POD file.
 * @return The Corresponding Isgl3dMaterial.
 */
- (Isgl3dMaterial *) materialWithName:(NSString *)materialName;

/**
 * Returns the Isgl3dNodeCamera corresponding to a camera index (as defined in the POD file).
 * All camera indices can be obtained via printPODInfo.
 * @param cameraIndex the index of the camera as defined in the POD file.
 * @return The Corresponding Isgl3dCamera.
 */
- (Isgl3dNodeCamera *) cameraAtIndex:(unsigned int)cameraIndex;

/**
 * Returns the Isgl3dLight corresponding to a light index (as defined in the POD file).
 * All light indices can be obtained via printPODInfo.
 * @param lightIndex the index of the light as defined in the POD file.
 * @return The Corresponding Isgl3dLight.
 */
- (Isgl3dLight *) lightAtIndex:(unsigned int)lightIndex;


/**
 * Takes the transformation matrix of a node in the POD file and applies it to
 * and Isgl3dLight. All node names can be obtained via printPODInfo.
 * @param light The Isgl3dLight to be configured.
 * @param nodeName The name of the node as defined in the POD file.
 */
- (void)configureLight:(Isgl3dLight *)light fromNode:(NSString *)nodeName;

/**
 * Overrides a texture file name, defined in the POD file, with another user-defined one.
 * @param podTextureFileName The original texture file name as defined in the POD data.
 * @param replacementFileName The user-defined texture file name.
 */
- (void)modifyTexture:(NSString *)podTextureFileName withTexture:(NSString *)replacementFileName;

- (id)objectForKeyedSubscript:(id)key;

@end
