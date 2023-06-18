/*!
*  @autor : Lukasz Pycz (xpyczl00)
 * @file gpu.cpp
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>


typedef struct triangle {
    OutVertex points[3];
} triangle_t;


void getVertexID(VertexArray const& vao, InVertex& inVertex, uint32_t v)
{
    if (vao.indexBuffer == nullptr)
    {
        //non-indexed
        inVertex.gl_VertexID = v;
    }
    else
    {   //indexed
        switch (vao.indexType)
        {
        case IndexType::UINT32:
            uint32_t* indexes = (uint32_t*)vao.indexBuffer;
            inVertex.gl_VertexID = indexes[v];
            break;

        case IndexType::UINT16:
            uint16_t* indexes = (uint16_t*)vao.indexBuffer;
            inVertex.gl_VertexID = indexes[v];
            break;

        case IndexType::UINT8:
            uint8_t* indexes = (uint8_t*)vao.indexBuffer;
            inVertex.gl_VertexID = indexes[v];
            break;

        }
    }
}

void setVertexAttributes(VertexAttrib const attrib[maxAttributes], InVertex& inVertex)
{
    for (uint32_t i = 0; i < maxAttributes; i++)
    {
        uint32_t ID = inVertex.gl_VertexID;
        uint64_t stride = (attrib[i].stride);
        uint64_t offset = (attrib[i].offset);
        uint8_t* buffer = (uint8_t*)attrib[i].bufferData;
        uint8_t* ptr;

        switch (attrib[i].type)
        {
        case AttributeType::EMPTY:
            continue;

        case AttributeType::FLOAT:
            ptr = buffer + offset + stride * ID;
            inVertex.attributes[i].v1 = *((float*)ptr);
            break;

        case AttributeType::VEC2:
            for (uint32_t j = 0; j < 2; j++)
            {
                ptr = buffer + offset + stride * ID + j * 4;
                inVertex.attributes[i].v2[j] = *((float*)ptr);
            }
            break;

        case AttributeType::VEC3:
            for (uint32_t j = 0; j < 3; j++)
            {
                ptr = buffer + offset + stride * ID + j * 4;
                inVertex.attributes[i].v3[j] = *((float*)ptr);
            }
            break;

        case AttributeType::VEC4:
            for (uint32_t j = 0; j < 4; j++)
            {
                ptr = buffer + offset + stride * ID + j * 4;
                inVertex.attributes[i].v4[j] = *((float*)ptr);
            }
            break;
        }
    }
}

void runVertexAssembly(VertexArray const& vao, InVertex& inVertex, uint32_t v)
{
    getVertexID(vao, inVertex, v);
    setVertexAttributes(vao.vertexAttrib, inVertex);
}

void runPrimitiveAssembly(triangle_t& Triangle, VertexArray const& vao, Program const& prg,
    uint32_t v)
{
    //Loop thorough all vertexes in a triangle
    for (uint32_t i = 0; i < 3; i++)
    {
        InVertex inVertex;
        runVertexAssembly(vao, inVertex, v + i);
        prg.vertexShader(Triangle.points[i], inVertex, prg.uniforms);
    }
}

void perspectiveDivision(triangle_t& Triangle)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Triangle.points[i].gl_Position[j] /= Triangle.points[i].gl_Position[3];
        }
    }
}

void viewportTransformation(Frame frame, triangle_t& Triangle)
{
    for (int p = 0; p < 3; p++)
    {
        float* x = &(Triangle.points[p].gl_Position[0]);
        *x = (*x * 0.5 + 0.5) * frame.width;

        float* y = &(Triangle.points[p].gl_Position[1]);
        *y = (*y * 0.5 + 0.5) * frame.height;
    }
}


//! [drawImpl]
void drawImpl(GPUContext& ctx, uint32_t nofVertices) {
    (void)ctx;
    (void)nofVertices;


    for (uint32_t t = 0; t < nofVertices; t += 3)
    {
        triangle_t Triangle;
        runPrimitiveAssembly(Triangle, ctx.vao, ctx.prg, t);

        perspectiveDivision(Triangle);
        viewportTransformation(ctx.frame, Triangle);

        //rasterize(ctx.frame, Triangle, ctx.prg);
    }





    /// \todo Tato funkce vykreslí trojúhelníky podle daného nastavení.<br>
    /// ctx obsahuje aktuální stav grafické karty.
    /// Parametr "nofVertices" obsahuje počet vrcholů, který by se měl vykreslit (3 pro jeden trojúhelník).<br>
    /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
}
//! [drawImpl]

/**
 * @brief This function reads color from texture.
 *
 * @param texture texture
 * @param uv uv coordinates
 *
 * @return color 4 floats
 */
glm::vec4 read_texture(Texture const& texture, glm::vec2 uv) {
    if (!texture.data)return glm::vec4(0.f);
    auto uv1 = glm::fract(uv);
    auto uv2 = uv1 * glm::vec2(texture.width - 1, texture.height - 1) + 0.5f;
    auto pix = glm::uvec2(uv2);
    //auto t   = glm::fract(uv2);
    glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 1.f);
    for (uint32_t c = 0;c < texture.channels;++c)
        color[c] = texture.data[(pix.y * texture.width + pix.x) * texture.channels + c] / 255.f;
    return color;
}

/**
 * @brief This function clears framebuffer.
 *
 * @param ctx GPUContext
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 * @param a alpha channel
 */
void clear(GPUContext& ctx, float r, float g, float b, float a) {
    auto& frame = ctx.frame;
    auto const nofPixels = frame.width * frame.height;
    for (size_t i = 0;i < nofPixels;++i) {
        frame.depth[i] = 10e10f;
        frame.color[i * 4 + 0] = static_cast<uint8_t>(glm::min(r * 255.f, 255.f));
        frame.color[i * 4 + 1] = static_cast<uint8_t>(glm::min(g * 255.f, 255.f));
        frame.color[i * 4 + 2] = static_cast<uint8_t>(glm::min(b * 255.f, 255.f));
        frame.color[i * 4 + 3] = static_cast<uint8_t>(glm::min(a * 255.f, 255.f));
    }
}

