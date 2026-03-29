
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{

    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);


    Out.TexCoord = In.TexCoord;
    Out.Position = mul(In.Position, wvp);
    Out.Diffuse = In.Diffuse * Material.Diffuse;
    Out.Depth = Out.Position.z;
}


