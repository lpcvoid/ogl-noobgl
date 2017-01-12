#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <glm/glm.hpp>
#include "nglCommon.h"
#include "nglMesh.h"
#include <memory>


using namespace ngl;
using namespace scene;

#ifndef nglCGFDefs_h__
#define nglCGFDefs_h__

namespace ngl {

	namespace loaders {
	
		namespace cgf {

			typedef enum 
			{
				ChunkType_ANY     = 0,

				ChunkType_Mesh      = 0xCCCC0000,
				ChunkType_Helper,
				ChunkType_VertAnim,
				ChunkType_BoneAnim,
				ChunkType_GeomNameList, // obsolete
				ChunkType_BoneNameList,
				ChunkType_MtlList,      // obsolete
				ChunkType_MRM,          // obsolete
				ChunkType_SceneProps,
				ChunkType_Light,
				ChunkType_PatchMesh,    // not implemented
				ChunkType_Node,
				ChunkType_Mtl,
				ChunkType_Controller,
				ChunkType_Timing,
				ChunkType_BoneMesh,
				ChunkType_BoneLightBinding, // describes the lights binded to bones
				ChunkType_MeshMorphTarget,  // describes a morph target of a mesh chunk
				ChunkType_BoneInitialPos,   // describes the initial position (4x3 matrix) of each bone; just an array of 4x3 matrices
				ChunkType_SourceInfo, // describes the source from which the cgf was exported: source max file, machine and user
				ChunkType_MtlName, // material name
				ChunkType_ExportFlags, // Special export flags.
				ChunkType_DataStream, // Stream data.
				ChunkType_MeshSubsets, // Array of mesh subsets.
				ChunkType_MeshPhysicsData, // Physicalized mesh data.

				// these are the new compiled chunks for characters
				ChunkType_CompiledBones = 0xACDC0000,
				ChunkType_CompiledPhysicalBones,
				ChunkType_CompiledMorphTargets,
				ChunkType_CompiledPhysicalProxies,
				ChunkType_CompiledIntFaces,
				ChunkType_CompiledIntSkinVertices,
				ChunkType_CompiledExt2IntMap,

				ChunkType_BreakablePhysics = 0xAAFC0000,
				ChunkType_FaceMap,
				ChunkType_MotionParameters,
				ChunkType_FootPlantInfo,
				ChunkType_BonesBoxes,
				ChunkType_FoliageInfo,
				ChunkType_Timestamp,
				ChunkType_GlobalAnimationHeaderCAF,
				ChunkType_GlobalAnimationHeaderAIM
			} ChunkTypes ;

			struct CHUNK_HEADER_0744
			{
				ChunkTypes ChunkType;
				int ChunkVersion;
				int FileOffset;
				int ChunkID; //new in version 0x0744
			};


			struct NODE_CHUNK_DESC_0823
			{
				enum {VERSION = 0x0823};

				CHUNK_HEADER_0744*  chdr;

				char    name[64];

				int     ObjectID;   // ID of this node's object chunk (if present)
				int     ParentID;   // chunk ID of the parent Node's chunk
				int     nChildren;    // # of children Nodes
				int     MatID;      // Material chunk No

				bool    IsGroupHead;
				bool    IsGroupMember;

				float tm[4][4];       // transformation matrix
				nglVec3  pos;      // pos component of matrix
				//CryQuat   rot;      // rotation component of matrix
				nglVec3  scl;      // scale component of matrix

				int     pos_cont_id;  // position controller chunk id
				int     rot_cont_id;  // rotation controller chunk id
				int     scl_cont_id;  // scale controller chunk id

				int     PropStrLen;   // lenght of the property string

			};


			enum HelperTypes
			{
				HP_POINT=0,
				HP_DUMMY=1,
				HP_XREF=2,
				HP_CAMERA=3,
				HP_GEOMETRY=4
			};


			struct HELPER_CHUNK_DESC_0744
			{
				enum {VERSION = 0x0744};
				CHUNK_HEADER_0744  chdr;

				HelperTypes type;     // one of the HelperTypes values
				nglVec3  size;     // size in local x,y,z axises (for dummy only)


			};

			struct TextureMap3
			{
				char  name[128];  // Name of file.

				unsigned char type;   // Mapping type.
				unsigned char flags;  // Mapping type.
				unsigned char Amount;
				unsigned char Reserved[32];

				//tiling and mirror
				bool  utile;
				bool  umirror;
				bool  vtile;
				bool  vmirror;

				int   nthFrame; // Update reflection every Nth frame.
				int   refSize;  // Reflection size.
				float refBlur;  // Reflection blur.

				//texture position values
				float uoff_val;
				float uscl_val;
				float urot_val;
				float voff_val;
				float vscl_val;
				float vrot_val;
				float wrot_val;

				//texture position controller chunk id's (if not animated they are -1)
				int uoff_ctrlID;
				int uscl_ctrlID;
				int urot_ctrlID;
				int voff_ctrlID;
				int vscl_ctrlID;
				int vrot_ctrlID;
				int wrot_ctrlID;
			};

			struct MaterialCGF {

				enum MTL_CHUNK_FLAGS
				{
					MTLFLAG_WIRE =    0x001,
					MTLFLAG_2SIDED =  0x002,
					MTLFLAG_FACEMAP = 0x004,
					MTLFLAG_FACETED = 0x008,
					MTLFLAG_ADDITIVE= 0x010,
					MTLFLAG_SUBTRACTIVE= 0x020,
					MTLFLAG_CRYSHADER= 0x040,
					MTLFLAG_PHYSICALIZE= 0x080,
					MTLFLAG_DETAILDECAL = 0x0100,
					MTLFLAG_USEGLOSSINESS = 0x0200,
				};

				char name[128]; // Material name;
				int nFlags;  // Material flags.
				int nPhysicalizeType; //PHYS_GEOM_TYPE_DEFAULT

				nglRGB col_d;    //diffuse color
				nglRGB col_s;    //specular color
				nglRGB col_a;    //ambient color

				TextureMap3 texture_diffuse;

				// Array of sub materials.
				std::vector<MaterialCGF*> subMaterials;

				//////////////////////////////////////////////////////////////////////////
				// Used internally.
				int nChunkId;
				//////////////////////////////////////////////////////////////////////////

			};

			struct NodeCGF {

				enum ENodeType
				{
					NODE_MESH,
					NODE_LIGHT,
					NODE_HELPER,
				};

				ENodeType type;
				char      name[64];
				std::string    properties;
				nglMat4  localTM;      // Local space transformation matrix.
				nglMat4  worldTM;      // World space transformation matrix.
				nglVec3      pos;		    // pos component of matrix
				//CryQuat   rot;	        // rotation component of matrix
				nglVec3      scl;		    // scale component of matrix
				NodeCGF* pParent;      // Pointer to parent node.
				NodeCGF* pSharedMesh;  // Not NULL if this node is sharing mesh and physics from referenced Node.
				Mesh*    pMesh;        // Pointer to mesh loaded for this node. (Only when type == NODE_MESH)

				HelperTypes helperType;  // Only relevant if type==NODE_HELPER
				nglVec3        helperSize;  // Only relevant if type==NODE_HELPER

				MaterialCGF* pMaterial; // Material node.


				//////////////////////////////////////////////////////////////////////////
				// Used internally.
				int nChunkId;       // Chunk id as loaded from CGF.
				int nParentChunkId; // Chunk id of parent Node.
				int nObjectChunkId; // Chunk id of the corresponding mesh.
				int	pos_cont_id;	// position controller chunk id
				int	rot_cont_id;	// rotation controller chunk id
				int scl_cont_id;	// scale controller chunk id  
				//////////////////////////////////////////////////////////////////////////

				// True if have not identity world matrix.
				bool bIdentityMatrix;
				// True when this not is invisible physics proxy.
				bool bPhysicsProxy;

			};




		}
	
	}



}


#endif // nglCGFDefs_h__