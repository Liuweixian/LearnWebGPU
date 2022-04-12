struct VertexOutput
{
    @builtin(position) ClipPosition: vec4<f32>,
    @location(0) Color: vec3<f32>,
};

@stage(vertex)
fn vs_main(@builtin(vertex_index) inVertexIndex: u32) ->VertexOutput
{
    var out : VertexOutput;
    let x = f32(1 - i32(inVertexIndex)) * 0.8;
    let y = f32(i32(inVertexIndex & 1u) * 2 - 1) * 0.8;
    out.ClipPosition = vec4<f32>(x, y, 0.0, 1.0);
    out.Color = vec3<f32>(x, y, 0.0);
    return out;
}