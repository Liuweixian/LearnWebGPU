struct VertexInput
{
    @location(0) Position: vec2<f32>,
    @location(1) Color: vec3<f32>,
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
    out.ClipPosition = vec4<f32>(inVertex.Position, 0.0, 1.0);
    out.Color = inVertex.Color;
    return out;
}
