struct VertexIn 
{
    @location(0) aPos : vec2<f32>,
    @location(1) aCol : vec3<f32>,
};
struct VertexOut 
{
    @location(0) vCol : vec3<f32>,
    @builtin(position) Position : vec4<f32>,
};
@stage(vertex)
fn vs_main(input : VertexIn) -> VertexOut {
    var output : VertexOut;
    output.Position = vec4<f32>(vec3<f32>(input.aPos, 1.0), 1.0);
    output.vCol = input.aCol;
    return output;
}