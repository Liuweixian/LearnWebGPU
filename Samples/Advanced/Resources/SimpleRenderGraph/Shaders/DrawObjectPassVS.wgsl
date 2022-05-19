struct VertexInput
{
    @location(0) Position: vec3<f32>,
    @location(1) Normal: vec3<f32>,
    @location(2) UV: vec2<f32>,
};

struct VertexOutput
{
    @builtin(position) ClipPosition: vec4<f32>,
    @location(0) Color: vec3<f32>,
};

@stage(vertex)
fn vs_main(inVertex: VertexInput) ->VertexOutput
{
    var out : VertexOutput;
    out.ClipPosition = vec4(inVertex.Position, 1.0);
    out.Color = inVertex.Position;
    return out;
}
