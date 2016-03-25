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
 134;
 0.00000;5.73373;17.97566;,
 12.64746;5.73373;12.71071;,
 12.64746;-0.39926;12.71071;,
 0.00000;-0.39926;17.97566;,
 10.11799;5.73373;10.16858;,
 0.00000;5.73373;14.38054;,
 0.00000;-0.39926;14.38054;,
 10.11799;-0.39926;10.16858;,
 17.88620;5.73373;0.00000;,
 17.88620;-0.39926;-0.00000;,
 14.30897;5.73373;0.00000;,
 14.30897;-0.39926;-0.00000;,
 12.64746;5.73373;-12.71071;,
 12.64746;-0.39926;-12.71071;,
 10.11799;5.73373;-10.16858;,
 10.11799;-0.39926;-10.16858;,
 0.00000;5.73373;-17.97566;,
 -0.00000;-0.39926;-17.97566;,
 0.00000;5.73373;-14.38054;,
 -0.00000;-0.39926;-14.38054;,
 -12.64747;5.73373;-12.71071;,
 -12.64747;-0.39926;-12.71071;,
 -10.11799;5.73373;-10.16857;,
 -10.11799;-0.39926;-10.16857;,
 -17.88620;5.73373;0.00000;,
 -17.88620;-0.39926;0.00000;,
 -14.30897;5.73373;0.00000;,
 -14.30897;-0.39926;0.00000;,
 -12.64747;5.73373;12.71071;,
 -12.64747;-0.39926;12.71071;,
 -10.11799;5.73373;10.16858;,
 -10.11799;-0.39926;10.16858;,
 0.00000;5.73373;17.97566;,
 0.00000;-0.39926;17.97566;,
 -10.11799;5.73373;10.16858;,
 -10.11799;-0.39926;10.16858;,
 0.00000;5.73373;14.38054;,
 0.00000;-0.39926;14.38054;,
 -11.32795;4.91904;-12.33864;,
 -5.20106;4.91904;-10.04539;,
 -5.06157;4.91904;-5.65849;,
 -9.23065;4.91904;-5.78455;,
 -0.04933;4.91904;-9.52373;,
 -0.01390;4.91904;-5.32447;,
 -0.00091;4.91904;-0.31816;,
 -4.74708;4.91904;-0.31671;,
 -8.74496;4.91904;-0.31512;,
 11.32795;4.91904;-12.33864;,
 9.26943;4.91904;-5.78237;,
 5.05043;4.91904;-5.65795;,
 5.11491;4.91904;-10.04526;,
 8.79314;4.91904;-0.31267;,
 4.75694;4.91904;-0.31590;,
 11.32795;4.91904;11.71463;,
 5.14975;4.91904;9.35719;,
 5.05914;4.91904;5.01806;,
 9.27160;4.91904;5.15495;,
 -0.01013;4.91904;8.82756;,
 -0.00084;4.91904;4.67623;,
 -11.32795;4.91904;11.71463;,
 -9.22848;4.91904;5.15277;,
 -5.05286;4.91904;5.01752;,
 -5.16622;4.91904;9.35705;,
 -8.62935;4.91903;-13.35103;,
 -4.37025;4.91903;-12.51500;,
 -5.20106;4.91904;-10.04539;,
 -5.93074;4.91902;-14.36342;,
 -2.69752;4.91902;-13.81695;,
 -0.18525;4.91903;-13.24363;,
 -0.11671;4.91903;-12.09698;,
 -0.04933;4.91904;-9.52373;,
 -0.53353;4.91900;-16.38821;,
 2.43184;4.91901;-15.37582;,
 -0.31123;4.91902;-14.70089;,
 -3.23214;4.91901;-15.37582;,
 5.39721;4.91902;-14.36342;,
 2.25846;4.91902;-13.81695;,
 11.32795;4.91904;-12.33864;,
 5.11491;4.91904;-10.04526;,
 4.12571;4.91903;-12.51500;,
 8.36258;4.91903;-13.35103;,
 11.32795;4.91904;11.71463;,
 8.46711;4.91903;12.53472;,
 4.22153;4.91903;11.71471;,
 5.14975;4.91904;9.35719;,
 5.60626;4.91902;13.35480;,
 2.43049;4.91902;12.87643;,
 -0.04008;4.91903;12.35253;,
 -0.02525;4.91903;11.30471;,
 -0.01013;4.91904;8.82756;,
 -0.11544;4.91900;14.99496;,
 -2.91857;4.91901;14.17488;,
 -0.06734;4.91902;13.62816;,
 2.74541;4.91901;14.17488;,
 -5.72170;4.91902;13.35480;,
 -2.52549;4.91902;12.87643;,
 -11.32795;4.91904;11.71463;,
 -5.16622;4.91904;9.35705;,
 -4.27444;4.91903;11.71471;,
 -8.52483;4.91903;12.53472;,
 -12.46169;4.91903;8.70232;,
 -11.65921;4.91903;4.19280;,
 -9.22848;4.91904;5.15277;,
 -13.59542;4.91902;5.69000;,
 -13.01689;4.91902;2.30951;,
 -12.43059;4.91903;-0.31986;,
 -11.25797;4.91903;-0.31695;,
 -8.74496;4.91904;-0.31512;,
 -15.86288;4.91900;-0.33463;,
 -14.72915;4.91901;-3.33563;,
 -13.97333;4.91902;-0.32520;,
 -14.72915;4.91901;2.67768;,
 -13.59542;4.91902;-6.33663;,
 -13.01689;4.91902;-2.95214;,
 -9.23065;4.91904;-5.78455;,
 -11.65921;4.91903;-4.82718;,
 -12.46169;4.91903;-9.33764;,
 12.59468;4.91903;-9.33110;,
 11.78112;4.91903;-4.82119;,
 9.26943;4.91904;-5.78237;,
 13.86140;4.91902;-6.32357;,
 13.23577;4.91902;-2.94138;,
 12.61529;4.91903;-0.31078;,
 11.37434;4.91903;-0.31123;,
 8.79314;4.91904;-0.31267;,
 16.39484;4.91900;-0.30849;,
 15.12812;4.91901;2.69729;,
 14.28364;4.91902;-0.30996;,
 15.12812;4.91901;-3.31603;,
 13.86140;4.91902;5.70307;,
 13.23577;4.91902;2.32027;,
 9.27160;4.91904;5.15495;,
 11.78112;4.91903;4.19879;,
 12.59468;4.91903;8.70885;;
 
 96;
 4;3,2,1,0;,
 4;7,6,5,4;,
 4;2,9,8,1;,
 4;11,7,4,10;,
 4;9,13,12,8;,
 4;15,11,10,14;,
 4;13,17,16,12;,
 4;19,15,14,18;,
 4;17,21,20,16;,
 4;23,19,18,22;,
 4;21,25,24,20;,
 4;27,23,22,26;,
 4;25,29,28,24;,
 4;31,27,26,30;,
 4;29,33,32,28;,
 4;6,35,34,5;,
 4;1,4,5,0;,
 4;6,7,2,3;,
 4;8,10,4,1;,
 4;7,11,9,2;,
 4;12,14,10,8;,
 4;11,15,13,9;,
 4;16,18,14,12;,
 4;15,19,17,13;,
 4;20,22,18,16;,
 4;19,23,21,17;,
 4;24,26,22,20;,
 4;23,27,25,21;,
 4;28,30,26,24;,
 4;27,31,29,25;,
 4;32,36,30,28;,
 4;31,37,33,29;,
 4;41,40,39,38;,
 4;39,40,43,42;,
 4;43,40,45,44;,
 4;45,40,41,46;,
 4;50,49,48,47;,
 4;48,49,52,51;,
 4;52,49,43,44;,
 4;43,49,50,42;,
 4;56,55,54,53;,
 4;54,55,58,57;,
 4;58,55,52,44;,
 4;52,55,56,51;,
 4;62,61,60,59;,
 4;60,61,45,46;,
 4;45,61,58,44;,
 4;58,61,62,57;,
 4;65,64,63,38;,
 4;63,64,67,66;,
 4;67,64,69,68;,
 4;69,64,65,70;,
 4;74,73,72,71;,
 4;72,73,76,75;,
 4;76,73,67,68;,
 4;67,73,74,66;,
 4;80,79,78,77;,
 4;78,79,69,70;,
 4;69,79,76,68;,
 4;76,79,80,75;,
 4;84,83,82,81;,
 4;82,83,86,85;,
 4;86,83,88,87;,
 4;88,83,84,89;,
 4;93,92,91,90;,
 4;91,92,95,94;,
 4;95,92,86,87;,
 4;86,92,93,85;,
 4;99,98,97,96;,
 4;97,98,88,89;,
 4;88,98,95,87;,
 4;95,98,99,94;,
 4;102,101,100,96;,
 4;100,101,104,103;,
 4;104,101,106,105;,
 4;106,101,102,107;,
 4;111,110,109,108;,
 4;109,110,113,112;,
 4;113,110,104,105;,
 4;104,110,111,103;,
 4;116,115,114,38;,
 4;114,115,106,107;,
 4;106,115,113,105;,
 4;113,115,116,112;,
 4;119,118,117,77;,
 4;117,118,121,120;,
 4;121,118,123,122;,
 4;123,118,119,124;,
 4;128,127,126,125;,
 4;126,127,130,129;,
 4;130,127,121,122;,
 4;121,127,128,120;,
 4;133,132,131,81;,
 4;131,132,123,124;,
 4;123,132,130,122;,
 4;130,132,133,129;;
 
 MeshMaterialList {
  2;
  96;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.200000;0.200000;0.200000;;
   TextureFilename {
    "data\\TEXTURE\\wall004.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.700000;0.700000;0.700000;;
  }
 }
 MeshNormals {
  98;
  0.000000;0.000000;1.000000;,
  0.708353;0.000000;0.705858;,
  1.000000;0.000000;0.000000;,
  0.708353;0.000000;-0.705858;,
  0.000000;0.000000;-1.000000;,
  -0.708353;0.000000;-0.705858;,
  -1.000000;0.000000;0.000000;,
  -0.708353;0.000000;0.705858;,
  0.000000;0.000000;-1.000000;,
  -0.708353;0.000000;-0.705858;,
  -0.708353;0.000000;0.705858;,
  0.000000;0.000000;1.000000;,
  0.708353;0.000000;0.705858;,
  1.000000;0.000000;0.000000;,
  0.708353;0.000000;-0.705858;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  -0.000000;1.000000;-0.000000;,
  0.000000;1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  -0.000000;1.000000;0.000000;,
  0.000000;1.000000;-0.000004;,
  0.000000;1.000000;-0.000008;,
  -0.000000;1.000000;-0.000004;,
  -0.000001;1.000000;0.000005;,
  0.000000;1.000000;0.000009;,
  0.000001;1.000000;0.000004;,
  -0.000004;1.000000;-0.000000;,
  -0.000007;1.000000;0.000000;,
  -0.000004;1.000000;0.000000;,
  0.000004;1.000000;0.000000;,
  0.000006;1.000000;-0.000000;,
  0.000003;1.000000;-0.000000;,
  0.000000;1.000000;-0.000002;,
  0.000000;1.000000;-0.000000;,
  -0.000000;1.000000;-0.000000;,
  -0.000002;1.000000;0.000000;,
  0.000001;1.000000;0.000000;,
  0.000000;1.000000;-0.000000;,
  -0.000000;1.000000;-0.000002;,
  -0.000000;1.000000;0.000002;,
  -0.000000;1.000000;0.000000;,
  0.000001;1.000000;-0.000000;,
  -0.000001;1.000000;-0.000000;,
  0.000000;1.000000;0.000001;,
  0.000001;1.000000;-0.000005;,
  0.000000;1.000000;-0.000006;,
  -0.000000;1.000000;-0.000003;,
  -0.000000;1.000000;-0.000009;,
  -0.000000;1.000000;-0.000006;,
  0.000000;1.000000;-0.000009;,
  -0.000001;1.000000;-0.000005;,
  -0.000001;1.000000;0.000006;,
  -0.000000;1.000000;0.000007;,
  -0.000000;1.000000;0.000003;,
  0.000000;1.000000;0.000010;,
  0.000000;1.000000;0.000007;,
  -0.000000;1.000000;0.000010;,
  0.000001;1.000000;0.000005;,
  -0.000004;1.000000;-0.000001;,
  -0.000006;1.000000;-0.000000;,
  -0.000003;1.000000;0.000000;,
  -0.000008;1.000000;0.000000;,
  -0.000006;1.000000;0.000000;,
  -0.000008;1.000000;-0.000000;,
  -0.000005;1.000000;0.000001;,
  0.000004;1.000000;0.000001;,
  0.000005;1.000000;0.000000;,
  0.000003;1.000000;0.000000;,
  0.000007;1.000000;-0.000000;,
  0.000005;1.000000;-0.000000;,
  0.000007;1.000000;0.000000;,
  0.000004;1.000000;-0.000001;,
  -0.000000;1.000000;-0.000000;,
  -0.000000;1.000000;-0.000005;,
  -0.000000;1.000000;0.000005;,
  -0.000005;1.000000;0.000000;,
  0.000004;1.000000;0.000000;,
  0.000000;1.000000;-0.000001;,
  0.000001;1.000000;-0.000000;,
  0.000000;1.000000;0.000001;,
  -0.000001;1.000000;-0.000000;,
  0.000000;1.000000;-0.000007;,
  -0.000000;1.000000;-0.000007;,
  -0.000000;1.000000;0.000008;,
  0.000000;1.000000;0.000008;,
  -0.000006;1.000000;-0.000000;,
  -0.000007;1.000000;0.000000;,
  0.000006;1.000000;0.000000;,
  0.000006;1.000000;-0.000000;,
  -0.000001;1.000000;-0.000001;,
  0.000001;1.000000;-0.000001;,
  -0.000001;1.000000;0.000001;,
  0.000001;1.000000;0.000001;,
  0.000000;1.000000;-0.000009;,
  0.000000;1.000000;0.000011;,
  -0.000008;1.000000;0.000000;,
  0.000007;1.000000;0.000000;;
  96;
  4;0,1,1,0;,
  4;9,8,8,9;,
  4;1,2,2,1;,
  4;6,9,9,6;,
  4;2,3,3,2;,
  4;10,6,6,10;,
  4;3,4,4,3;,
  4;11,10,10,11;,
  4;4,5,5,4;,
  4;12,11,11,12;,
  4;5,6,6,5;,
  4;13,12,12,13;,
  4;6,7,7,6;,
  4;14,13,13,14;,
  4;7,0,0,7;,
  4;8,14,14,8;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;36,17,33,90;,
  4;33,17,34,78;,
  4;34,17,35,73;,
  4;35,17,36,81;,
  4;39,18,37,91;,
  4;37,18,38,79;,
  4;38,18,34,73;,
  4;34,18,39,78;,
  4;42,19,40,93;,
  4;40,19,41,80;,
  4;41,19,38,73;,
  4;38,19,42,79;,
  4;44,20,43,92;,
  4;43,20,35,81;,
  4;35,20,41,73;,
  4;41,20,44,80;,
  4;33,21,45,90;,
  4;45,21,46,82;,
  4;46,21,47,74;,
  4;47,21,33,78;,
  4;50,22,48,94;,
  4;48,22,49,83;,
  4;49,22,46,74;,
  4;46,22,50,82;,
  4;51,23,39,91;,
  4;39,23,47,78;,
  4;47,23,49,74;,
  4;49,23,51,83;,
  4;40,24,52,93;,
  4;52,24,53,84;,
  4;53,24,54,75;,
  4;54,24,40,80;,
  4;57,25,55,95;,
  4;55,25,56,85;,
  4;56,25,53,75;,
  4;53,25,57,84;,
  4;58,26,44,92;,
  4;44,26,54,80;,
  4;54,26,56,75;,
  4;56,26,58,85;,
  4;43,27,59,92;,
  4;59,27,60,86;,
  4;60,27,61,76;,
  4;61,27,43,81;,
  4;64,28,62,96;,
  4;62,28,63,87;,
  4;63,28,60,76;,
  4;60,28,64,86;,
  4;65,29,36,90;,
  4;36,29,61,81;,
  4;61,29,63,76;,
  4;63,29,65,87;,
  4;37,30,66,91;,
  4;66,30,67,88;,
  4;67,30,68,77;,
  4;68,30,37,79;,
  4;71,31,69,97;,
  4;69,31,70,89;,
  4;70,31,67,77;,
  4;67,31,71,88;,
  4;72,32,42,93;,
  4;42,32,68,79;,
  4;68,32,70,77;,
  4;70,32,72,89;;
 }
 MeshTextureCoords {
  134;
  0.000000;0.401715;,
  0.124603;0.401486;,
  0.124603;0.507086;,
  0.000000;0.507069;,
  0.124603;0.378961;,
  0.000000;0.379234;,
  0.000000;0.508835;,
  0.124603;0.508857;,
  0.250000;0.401255;,
  0.250000;0.507104;,
  0.250000;0.378687;,
  0.250000;0.508879;,
  0.375397;0.401486;,
  0.375397;0.507086;,
  0.375397;0.378961;,
  0.375397;0.508857;,
  0.500000;0.401715;,
  0.500000;0.507069;,
  0.500000;0.379234;,
  0.500000;0.508835;,
  0.624603;0.401486;,
  0.624603;0.507086;,
  0.624603;0.378961;,
  0.624603;0.508857;,
  0.750000;0.401255;,
  0.750000;0.507104;,
  0.750000;0.378687;,
  0.750000;0.508879;,
  0.875397;0.401486;,
  0.875397;0.507086;,
  0.875397;0.378961;,
  0.875397;0.508857;,
  1.000000;0.401715;,
  1.000000;0.507069;,
  -0.124603;0.378961;,
  -0.124603;0.508857;,
  1.000000;0.379234;,
  1.000000;0.508835;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.500000;0.500000;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.750000;0.250000;,
  0.750000;0.000000;,
  1.000000;0.500000;,
  0.750000;0.500000;,
  1.000000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.750000;,
  1.000000;0.750000;,
  0.500000;1.000000;,
  0.500000;0.750000;,
  0.000000;1.000000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.250000;1.000000;,
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
