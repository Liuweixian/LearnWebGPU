struct VertexIn
{
    @location(0) aPos : vec3<f32>,
    @location(1) aUV : vec2<f32>,
};
struct VertexOut
{
    @location(0) vCol : vec3<f32>,
    @builtin(position) Position : vec4<f32>,
};
@group(0) @binding(0) var<uniform> vpMatrix: mat4x4<f32>;
@group(0) @binding(1) var<uniform> objToWorldMatrix: mat4x4<f32>;
@stage(vertex)
fn vs_main(input : VertexIn) -> VertexOut
{
    var output : VertexOut;
    output.Position = vpMatrix * (objToWorldMatrix * vec4<f32>(input.aPos, 1.0));
    output.vCol = vec3<f32>(1.0f, 0.0f, 0.0f);
    return output;
}
