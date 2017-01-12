#include "glm/glm.hpp"
#include "gl/glew.h"
#include <stdlib.h>
#include "raxStringUtils.h"

#ifndef nglCommon_h__
#define nglCommon_h__


/************************************************************************/
/* OPTIONS                                                                     */
/************************************************************************/

#define NGL_RESOURCEMANAGER_DIR_SEPERATOR "/"

#define NGL_RENDER_SHADOWMAP_RES 8192

#define NGL_PI 3.1415926

#define NGL_DEGTORAD(Deg) ((Deg * NGL_PI) / 180.0)

#define NGL_RADTODEG(Rad) ((180.0 * Rad) / NGL_PI)

typedef glm::vec4 nglVec4;
typedef glm::vec3 nglVertex3P;
typedef glm::vec3 nglVec3;
typedef glm::vec2 nglVec2;
typedef glm::u8vec2 nglVec2U8;
typedef glm::u16vec2 nglVec2U16;
typedef glm::mat4 nglMat4;
typedef glm::mat3 nglMat3;
typedef glm::u8vec3 nglRGB;
typedef glm::vec3 nglRGB3F;
typedef nglVec2 nglUV;


//generate random color, normalized
inline nglVec3 GenerateRandomNormalizedColor(){

	nglVec3 col;
	col.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	col.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	col.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return col;
}

inline std::string Vec3ToString(nglVec3& vec){

	std::string res = "";
	res += "x:" + rax::utils::StringUtils::NumberToString<float>(vec.x);
	res += "y:" + rax::utils::StringUtils::NumberToString<float>(vec.y);
	res += "z:" + rax::utils::StringUtils::NumberToString<float>(vec.z);
	res += "\n";
	return res;

}


//#define cornflowerblue (100, 149, 237) // important

struct nglVertex3P3N {

	nglVertex3P position;
	//vertex normal 
	//http://en.wikipedia.org/wiki/Vertex_normal
	glm::vec3 normal;

};



// #pragma pack(push, 1)
// struct nglFace3US {
// 	//vertex indices
// 	GLushort v0,v1,v2;
// 
// 	nglFace3US operator= (const nglFace3US& other) {
// 		v0 = other.v0;
// 		v1 = other.v1;
// 		v2 = other.v2;
// 		return *this;
// 	}
// };
// #pragma pack(pop)



struct nglTriangle {

	nglVertex3P p0,p1,p2;

};

enum NGL_ARRAY_DATA_TYPE {

	NGL_ARRAY_DATA_TYPE_VERTEX = 1,
	NGL_ARRAY_DATA_TYPE_INDEX = 2,
	NGL_ARRAY_DATA_TYPE_NORMALS = 4,
	NGL_ARRAY_DATA_TYPE_UV = 8,
	NGL_ARRAY_DATA_TYPE_COLORS = 16

};




#endif // nglCommon_h__
