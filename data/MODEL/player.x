xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 165;
 0.00000;33.90000;0.00000;,
 2.70598;33.13879;-2.70598;,
 0.00000;33.13879;-3.82683;,
 0.00000;33.90000;0.00000;,
 3.82683;33.13879;0.00000;,
 0.00000;33.90000;0.00000;,
 2.70598;33.13879;2.70598;,
 0.00000;33.90000;0.00000;,
 0.00000;33.13879;3.82683;,
 0.00000;33.90000;0.00000;,
 -2.70598;33.13879;2.70598;,
 0.00000;33.90000;0.00000;,
 -3.82683;33.13879;0.00000;,
 0.00000;33.90000;0.00000;,
 -2.70598;33.13879;-2.70598;,
 0.00000;33.90000;0.00000;,
 0.00000;33.13879;-3.82683;,
 5.00000;30.97107;-5.00000;,
 0.00000;30.97107;-7.07107;,
 7.07107;30.97107;0.00000;,
 5.00000;30.97107;5.00000;,
 0.00000;30.97107;7.07107;,
 -5.00000;30.97107;5.00000;,
 -7.07107;30.97107;0.00000;,
 -5.00000;30.97107;-5.00000;,
 0.00000;30.97107;-7.07107;,
 6.53281;27.72683;-6.53281;,
 0.00000;27.72683;-9.23879;,
 9.23879;27.72683;0.00000;,
 6.53281;27.72683;6.53281;,
 0.00000;27.72683;9.23879;,
 -6.53282;27.72683;6.53281;,
 -9.23879;27.72683;0.00000;,
 -6.53282;27.72683;-6.53281;,
 0.00000;27.72683;-9.23879;,
 7.07107;23.90000;-7.07107;,
 0.00000;23.90000;-10.00000;,
 10.00000;23.90000;0.00000;,
 7.07107;23.90000;7.07107;,
 0.00000;23.90000;10.00000;,
 -7.07107;23.90000;7.07107;,
 -10.00000;23.90000;0.00000;,
 -7.07107;23.90000;-7.07107;,
 0.00000;23.90000;-10.00000;,
 6.53281;20.07316;-6.53281;,
 0.00000;20.07316;-9.23879;,
 9.23879;20.07316;0.00000;,
 6.53281;20.07316;6.53281;,
 0.00000;20.07316;9.23879;,
 -6.53282;20.07316;6.53281;,
 -9.23879;20.07316;0.00000;,
 -6.53282;20.07316;-6.53281;,
 0.00000;20.07316;-9.23879;,
 5.00000;16.82893;-5.00000;,
 0.00000;16.82893;-7.07107;,
 7.07107;16.82893;0.00000;,
 5.00000;16.82893;5.00000;,
 0.00000;16.82893;7.07107;,
 -5.00000;16.82893;5.00000;,
 -7.07107;16.82893;0.00000;,
 -5.00000;16.82893;-5.00000;,
 0.00000;16.82893;-7.07107;,
 2.70598;14.66121;-2.70598;,
 0.00000;14.66121;-3.82683;,
 3.82683;14.66121;0.00000;,
 2.70598;14.66121;2.70598;,
 0.00000;14.66121;3.82683;,
 -2.70598;14.66121;2.70598;,
 -3.82683;14.66121;0.00000;,
 -2.70598;14.66121;-2.70598;,
 0.00000;14.66121;-3.82683;,
 0.00000;13.90000;0.00000;,
 0.00000;13.90000;0.00000;,
 0.00000;13.90000;0.00000;,
 0.00000;13.90000;0.00000;,
 0.00000;13.90000;0.00000;,
 0.00000;13.90000;0.00000;,
 0.00000;13.90000;0.00000;,
 0.00000;13.90000;0.00000;,
 -10.00000;20.04867;-10.00000;,
 10.00000;20.04867;-10.00000;,
 10.00000;0.04867;-10.00000;,
 -10.00000;0.04867;-10.00000;,
 10.00000;20.04867;-10.00000;,
 10.00000;20.04867;10.00000;,
 10.00000;0.04867;10.00000;,
 10.00000;0.04867;-10.00000;,
 10.00000;20.04867;10.00000;,
 -10.00000;20.04867;10.00000;,
 -10.00000;0.04867;10.00000;,
 10.00000;0.04867;10.00000;,
 -10.00000;20.04867;10.00000;,
 -10.00000;20.04867;-10.00000;,
 -10.00000;0.04867;-10.00000;,
 -10.00000;0.04867;10.00000;,
 10.00000;20.04867;-10.00000;,
 -10.00000;20.04867;-10.00000;,
 -10.00000;0.04867;-10.00000;,
 10.00000;0.04867;-10.00000;,
 0.00000;21.01900;7.54400;,
 2.86700;22.19770;7.39926;,
 2.86700;23.18595;15.44795;,
 0.00000;22.00726;15.59267;,
 2.29360;22.76683;7.32938;,
 0.00000;21.82387;7.44517;,
 0.00000;22.81212;15.49385;,
 2.29360;23.75508;15.37807;,
 4.05455;25.04333;7.04987;,
 4.05455;26.03159;15.09853;,
 3.24364;25.04333;7.04987;,
 3.24364;26.03159;15.09853;,
 2.86700;27.88897;6.70048;,
 2.86700;28.87723;14.74915;,
 2.29360;27.31984;6.77035;,
 2.29360;28.30810;14.81902;,
 -0.00000;29.06766;6.55574;,
 -0.00000;30.05592;14.60441;,
 -0.00000;28.26282;6.65456;,
 -0.00000;29.25105;14.70325;,
 -2.86700;27.88897;6.70048;,
 -2.86700;28.87723;14.74915;,
 -2.29360;27.31984;6.77035;,
 -2.29360;28.30810;14.81902;,
 -4.05455;25.04333;7.04987;,
 -4.05455;26.03159;15.09853;,
 -3.24364;25.04333;7.04987;,
 -3.24364;26.03159;15.09853;,
 -2.86700;22.19770;7.39926;,
 -2.86700;23.18595;15.44795;,
 -2.29360;22.76683;7.32938;,
 -2.29360;23.75508;15.37807;,
 0.00000;21.01900;7.54400;,
 0.00000;22.00726;15.59267;,
 0.00000;21.82387;7.44517;,
 0.00000;22.81212;15.49385;,
 2.29360;22.76683;7.32938;,
 2.86700;22.19770;7.39926;,
 0.00000;22.00726;15.59267;,
 2.86700;23.18595;15.44795;,
 2.29360;23.75508;15.37807;,
 0.00000;22.81212;15.49385;,
 3.24364;25.04333;7.04987;,
 4.05455;25.04333;7.04987;,
 4.05455;26.03159;15.09853;,
 3.24364;26.03159;15.09853;,
 2.29360;27.31984;6.77035;,
 2.86700;27.88897;6.70048;,
 2.86700;28.87723;14.74915;,
 2.29360;28.30810;14.81902;,
 -0.00000;28.26282;6.65456;,
 -0.00000;29.06766;6.55574;,
 -0.00000;30.05592;14.60441;,
 -0.00000;29.25105;14.70325;,
 -2.29360;27.31984;6.77035;,
 -2.86700;27.88897;6.70048;,
 -2.86700;28.87723;14.74915;,
 -2.29360;28.30810;14.81902;,
 -3.24364;25.04333;7.04987;,
 -4.05455;25.04333;7.04987;,
 -4.05455;26.03159;15.09853;,
 -3.24364;26.03159;15.09853;,
 -2.29360;22.76683;7.32938;,
 -2.86700;22.19770;7.39926;,
 -2.86700;23.18595;15.44795;,
 -2.29360;23.75508;15.37807;;
 
 102;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 4;2,1,17,18;,
 4;1,4,19,17;,
 4;4,6,20,19;,
 4;6,8,21,20;,
 4;8,10,22,21;,
 4;10,12,23,22;,
 4;12,14,24,23;,
 4;14,16,25,24;,
 4;18,17,26,27;,
 4;17,19,28,26;,
 4;19,20,29,28;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;27,26,35,36;,
 4;26,28,37,35;,
 4;28,29,38,37;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;36,35,44,45;,
 4;35,37,46,44;,
 4;37,38,47,46;,
 4;38,39,48,47;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;45,44,53,54;,
 4;44,46,55,53;,
 4;46,47,56,55;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;54,53,62,63;,
 4;53,55,64,62;,
 4;55,56,65,64;,
 4;56,57,66,65;,
 4;57,58,67,66;,
 4;58,59,68,67;,
 4;59,60,69,68;,
 4;60,61,70,69;,
 3;63,62,71;,
 3;62,64,72;,
 3;64,65,73;,
 3;65,66,74;,
 3;66,67,75;,
 3;67,68,76;,
 3;68,69,77;,
 3;69,70,78;,
 4;79,80,81,82;,
 4;83,84,85,86;,
 4;87,88,89,90;,
 4;91,92,93,94;,
 4;91,84,95,96;,
 4;97,98,85,94;,
 4;99,100,101,102;,
 4;103,104,105,106;,
 4;100,107,108,101;,
 4;109,103,106,110;,
 4;107,111,112,108;,
 4;113,109,110,114;,
 4;111,115,116,112;,
 4;117,113,114,118;,
 4;115,119,120,116;,
 4;121,117,118,122;,
 4;119,123,124,120;,
 4;125,121,122,126;,
 4;123,127,128,124;,
 4;129,125,126,130;,
 4;127,131,132,128;,
 4;133,129,130,134;,
 4;99,104,135,136;,
 4;137,138,139,140;,
 4;136,135,141,142;,
 4;138,143,144,139;,
 4;142,141,145,146;,
 4;143,147,148,144;,
 4;146,145,149,150;,
 4;147,151,152,148;,
 4;150,149,153,154;,
 4;151,155,156,152;,
 4;154,153,157,158;,
 4;155,159,160,156;,
 4;158,157,161,162;,
 4;159,163,164,160;,
 4;162,161,104,99;,
 4;163,137,140,164;;
 
 MeshMaterialList {
  1;
  102;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  96;
  -0.000000;1.000000;0.000000;,
  0.000000;0.924735;-0.380611;,
  0.269132;0.924735;-0.269132;,
  0.380611;0.924735;0.000000;,
  0.269132;0.924735;0.269132;,
  0.000000;0.924735;0.380611;,
  -0.269132;0.924735;0.269132;,
  -0.380611;0.924735;0.000000;,
  -0.269132;0.924735;-0.269132;,
  0.000000;0.709230;-0.704977;,
  0.498494;0.709230;-0.498494;,
  0.704977;0.709230;0.000000;,
  0.498494;0.709230;0.498494;,
  0.000000;0.709230;0.704977;,
  -0.498494;0.709231;0.498494;,
  -0.704977;0.709231;0.000000;,
  -0.498494;0.709231;-0.498494;,
  0.000000;0.384551;-0.923104;,
  0.652733;0.384551;-0.652733;,
  0.923104;0.384551;0.000000;,
  0.652733;0.384551;0.652733;,
  0.000000;0.384551;0.923104;,
  -0.652733;0.384551;0.652733;,
  -0.923104;0.384551;0.000000;,
  -0.652733;0.384551;-0.652733;,
  0.000000;0.000000;-1.000000;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707107;,
  0.000000;0.000000;1.000000;,
  -0.707107;0.000000;0.707107;,
  -1.000000;0.000000;0.000000;,
  -0.707107;0.000000;-0.707107;,
  0.000000;-0.384551;-0.923104;,
  0.652733;-0.384551;-0.652733;,
  0.923104;-0.384551;-0.000000;,
  0.652733;-0.384551;0.652733;,
  0.000000;-0.384551;0.923104;,
  -0.652733;-0.384551;0.652733;,
  -0.923104;-0.384551;0.000000;,
  -0.652733;-0.384551;-0.652733;,
  0.000000;-0.709231;-0.704976;,
  0.498494;-0.709231;-0.498494;,
  0.704976;-0.709231;-0.000000;,
  0.498494;-0.709231;0.498494;,
  0.000000;-0.709231;0.704976;,
  -0.498493;-0.709231;0.498493;,
  -0.704976;-0.709231;0.000000;,
  -0.498493;-0.709231;-0.498493;,
  0.000000;-0.924735;-0.380611;,
  0.269132;-0.924735;-0.269132;,
  0.380611;-0.924735;-0.000000;,
  0.269132;-0.924735;0.269132;,
  0.000000;-0.924735;0.380611;,
  -0.269132;-0.924735;0.269132;,
  -0.380611;-0.924735;0.000000;,
  -0.269132;-0.924735;-0.269132;,
  -0.000000;-1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-0.992546;0.121870;,
  0.707107;-0.701836;0.086175;,
  1.000000;0.000000;-0.000000;,
  0.707106;0.701837;-0.086175;,
  0.000000;0.992546;-0.121870;,
  -0.707107;0.701836;-0.086175;,
  -1.000000;0.000000;-0.000000;,
  -0.707107;-0.701836;0.086175;,
  0.000000;0.992546;-0.121869;,
  -0.707107;0.701835;-0.086174;,
  -1.000000;-0.000000;0.000000;,
  -0.707108;-0.701835;0.086174;,
  0.000000;-0.992546;0.121868;,
  0.707107;-0.701835;0.086174;,
  1.000000;-0.000000;0.000000;,
  0.707107;0.701836;-0.086174;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-0.121876;-0.992545;,
  0.000002;-0.121871;-0.992546;,
  0.000000;0.121864;0.992547;,
  0.000003;0.121869;0.992546;,
  0.000005;-0.121867;-0.992547;,
  -0.000001;0.121868;0.992546;,
  0.000005;-0.121865;-0.992547;,
  -0.000003;0.121865;0.992547;,
  0.000000;-0.121864;-0.992547;,
  -0.000000;0.121868;0.992546;,
  -0.000006;-0.121865;-0.992547;,
  0.000003;0.121865;0.992547;,
  -0.000005;-0.121866;-0.992546;,
  0.000001;0.121868;0.992546;,
  -0.000002;-0.121871;-0.992546;,
  -0.000003;0.121869;0.992546;;
  102;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,25,33,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,33,41,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,41,49,56;,
  3;49,50,57;,
  3;50,51,57;,
  3;51,52,57;,
  3;52,53,57;,
  3;53,54,57;,
  3;54,55,57;,
  3;55,56,57;,
  3;56,49,57;,
  4;58,58,58,58;,
  4;59,59,59,59;,
  4;60,60,60,60;,
  4;77,77,77,77;,
  4;78,78,78,78;,
  4;79,79,79,79;,
  4;61,62,62,61;,
  4;70,69,69,70;,
  4;62,63,63,62;,
  4;71,70,70,71;,
  4;63,64,64,63;,
  4;72,71,71,72;,
  4;64,65,65,64;,
  4;73,72,72,73;,
  4;65,66,66,65;,
  4;74,73,73,74;,
  4;66,67,67,66;,
  4;75,74,74,75;,
  4;67,68,68,67;,
  4;76,75,75,76;,
  4;68,61,61,68;,
  4;69,76,76,69;,
  4;80,80,81,81;,
  4;82,83,83,82;,
  4;81,81,84,84;,
  4;83,85,85,83;,
  4;84,84,86,86;,
  4;85,87,87,85;,
  4;86,86,88,88;,
  4;87,89,89,87;,
  4;88,88,90,90;,
  4;89,91,91,89;,
  4;90,90,92,92;,
  4;91,93,93,91;,
  4;92,92,94,94;,
  4;93,95,95,93;,
  4;94,94,80,80;,
  4;95,82,82,95;;
 }
 MeshTextureCoords {
  165;
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
