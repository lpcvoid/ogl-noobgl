/************************************************************************/
/* ALWAYS IN SHADER                                                     */
/************************************************************************/

//projection matrix of observer
#define NGL_SHADER_UNIFORM_MAT4_PROJECTION "mat4_projection" 
//View matrix of observer
#define NGL_SHADER_UNIFORM_MAT4_VIEW "mat4_view" 
//Model Transformation matrix
#define NGL_SHADER_UNIFORM_MAT4_MODEL "mat4_model" 

/************************************************************************/
/* ONLY UNDER CERTAIN CIRCUMSTANCES                                     */
/************************************************************************/

#define NGL_SHADER_MATERIAL_COLOR_DIFFUSE "vec3_material_color_diffuse"
#define NGL_SHADER_MATERIAL_COLOR_SPECULAR "vec3_material_color_specular"
#define NGL_SHADER_MATERIAL_COLOR_AMBIENT "vec3_material_color_ambient"

#define NGL_SHADER_MATERIAL_LIGHT_COLOR "vec3_light_color"
#define NGL_SHADER_MATERIAL_LIGHT_POSITION "vec3_light_position"

#define NGL_SHADER_MATERIAL_LIGHT_AMBIENT "vec3_light_ambient"


/************************************************************************/
/* ATTRIBUTES                                                           */
/************************************************************************/

//the vertex data in model space which gets passed to shader.
#define NGL_SHADER_LOCATION_IN_VERTEX_VERTEXDATA 0
#define NGL_SHADER_IN_VERTEX_VERTEXDATA "in_vs_vertex"

//vertex uv coordinates
#define NGL_SHADER_LOCATION_IN_VERTEX_UVCOORDS 1
#define NGL_SHADER_IN_VERTEX_UVCOORDS "in_vs_uv"

//vertex shader normals
#define NGL_SHADER_LOCATION_IN_VERTEX_NORMALS 2
#define NGL_SHADER_IN_VERTEX_NORMALS "in_vs_normal"

//vertex shader colors
#define NGL_SHADER_LOCATION_IN_VERTEX_COLORS 3
#define NGL_SHADER_IN_VERTEX_COLORS "in_vs_colors"

//out variable of fragment shader. Not sure if we need this yet.
#define NGL_SHADER_OUT_FRAGMENT_FINALCOLOR "out_fs_finalcolor"

#define NGL_SHADER_ERROR 0xFFFFFFFF
#define NGL_SHADER_UNIFORM_NAME_MAX_LEN 64

#define NGL_SHADER_FILENAME_SUFFIX_VERTEX ".vs"
#define NGL_SHADER_FILENAME_SUFFIX_FRAGMENT ".fs"

/************************************************************************/
/* SHADER DEFINES                                                       */
/************************************************************************/

#define NGL_UBERSHADER_NAME "ubershader"

#define NGL_SHDR_TRANSFORM 0
#define NGL_SHDR_NORMALSINPUT 1
#define NGL_SHDR_SHADOWMAPPING 2
#define NGL_SHDR_DEPTHPASS 3
// #define NGL_ 0
// #define NGL_ 0
// #define NGL_ 0

const char* const NGL_SHDR_DEFINE_STRINGS[] = {"SHDR_TRANSFORM", "SHDR_NORMALSINPUT", "SHDR_SHADOWMAPPING", "SHDR_DEPTHPASS" };
#define NGL_SHDR_DEFINE_STRINGS_COUNT 4