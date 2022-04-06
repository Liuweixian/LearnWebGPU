struct VertexOutput
{
    @builtin(position) ClipPosition: vec4<f32>;
    @location(0) Color: vec3<f32>;
};

@stage(fragment)
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32>
{
    return vec4<f32>(in.Color, 1.0);
}