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
 82;
 0.00000;-11.91164;0.00000;,
 2.12826;-11.91163;-2.12826;,
 0.00000;-11.91165;-3.00982;,
 3.00982;-11.91164;0.00000;,
 2.12826;-11.91163;2.12826;,
 0.00000;-11.91164;3.00982;,
 -2.12827;-11.91163;2.12826;,
 -3.00983;-11.91164;0.00000;,
 -2.12827;-11.91163;-2.12826;,
 2.33188;-11.75940;-2.33188;,
 0.00000;-11.75940;-3.29778;,
 3.29777;-11.75939;-0.00001;,
 2.33188;-11.75940;2.33188;,
 0.00000;-11.75940;3.29778;,
 -2.33188;-11.75940;2.33188;,
 -3.29778;-11.75939;-0.00001;,
 -2.33188;-11.75940;-2.33188;,
 2.50449;-11.32584;-2.50449;,
 0.00000;-11.32583;-3.54189;,
 3.54188;-11.32584;-0.00000;,
 2.50449;-11.32583;2.50449;,
 0.00000;-11.32583;3.54189;,
 -2.50450;-11.32583;2.50449;,
 -3.54189;-11.32584;-0.00000;,
 -2.50450;-11.32584;-2.50449;,
 2.61983;-10.67701;-2.61983;,
 0.00000;-10.67701;-3.70500;,
 3.70500;-10.67701;-0.00000;,
 2.61983;-10.67702;2.61983;,
 0.00000;-10.67701;3.70500;,
 -2.61983;-10.67702;2.61983;,
 -3.70501;-10.67701;-0.00000;,
 -2.61983;-10.67701;-2.61983;,
 2.64359;-9.91164;-2.66033;,
 0.00000;-9.91164;-3.76227;,
 3.77902;-9.91164;-0.00000;,
 2.66033;-9.91163;2.66033;,
 0.00000;-9.91163;3.76227;,
 -2.66034;-9.91163;2.66033;,
 -3.76229;-9.91164;-0.00000;,
 -2.66034;-9.91164;-2.66033;,
 2.64359;6.08836;-2.66033;,
 0.00000;6.08836;-3.76227;,
 3.77902;6.08836;-0.00000;,
 2.66033;6.08836;2.66033;,
 0.00000;6.08836;3.76227;,
 -2.66034;6.08836;2.66033;,
 -3.76229;6.08836;-0.00000;,
 -2.66034;6.08836;-2.66033;,
 2.61983;6.85373;-2.61983;,
 0.00000;6.85373;-3.70500;,
 3.70500;6.85373;-0.00000;,
 2.61983;6.85373;2.61983;,
 0.00000;6.85373;3.70500;,
 -2.61983;6.85373;2.61983;,
 -3.70501;6.85373;-0.00000;,
 -2.61983;6.85373;-2.61983;,
 2.50449;7.50257;-2.50449;,
 0.00000;7.50257;-3.54189;,
 3.54188;7.50257;-0.00000;,
 2.50449;7.50257;2.50449;,
 0.00000;7.50257;3.54189;,
 -2.50450;7.50257;2.50449;,
 -3.54189;7.50257;-0.00000;,
 -2.50450;7.50257;-2.50449;,
 2.33188;7.93612;-2.33188;,
 0.00000;7.93612;-3.29778;,
 3.29777;7.93612;-0.00000;,
 2.33188;7.93612;2.33188;,
 0.00000;7.93612;3.29778;,
 -2.33188;7.93612;2.33188;,
 -3.29778;7.93612;-0.00000;,
 -2.33188;7.93612;-2.33188;,
 2.12826;8.08836;-2.12826;,
 0.00000;8.08836;-3.00982;,
 3.00982;8.08836;-0.00000;,
 2.12826;8.08836;2.12826;,
 0.00000;8.08836;3.00982;,
 -2.12827;8.08836;2.12826;,
 -3.00983;8.08836;-0.00000;,
 -2.12827;8.08836;-2.12826;,
 0.00000;18.62283;-0.00000;;
 
 88;
 3;2,1,0;,
 3;1,3,0;,
 3;3,4,0;,
 3;4,5,0;,
 3;5,6,0;,
 3;6,7,0;,
 3;7,8,0;,
 3;8,2,0;,
 4;10,9,1,2;,
 4;9,11,3,1;,
 4;11,12,4,3;,
 4;12,13,5,4;,
 4;13,14,6,5;,
 4;14,15,7,6;,
 4;15,16,8,7;,
 4;16,10,2,8;,
 4;18,17,9,10;,
 4;17,19,11,9;,
 4;19,20,12,11;,
 4;20,21,13,12;,
 4;21,22,14,13;,
 4;22,23,15,14;,
 4;23,24,16,15;,
 4;24,18,10,16;,
 4;26,25,17,18;,
 4;25,27,19,17;,
 4;27,28,20,19;,
 4;28,29,21,20;,
 4;29,30,22,21;,
 4;30,31,23,22;,
 4;31,32,24,23;,
 4;32,26,18,24;,
 4;34,33,25,26;,
 4;33,35,27,25;,
 4;35,36,28,27;,
 4;36,37,29,28;,
 4;37,38,30,29;,
 4;38,39,31,30;,
 4;39,40,32,31;,
 4;40,34,26,32;,
 4;42,41,33,34;,
 4;41,43,35,33;,
 4;43,44,36,35;,
 4;44,45,37,36;,
 4;45,46,38,37;,
 4;46,47,39,38;,
 4;47,48,40,39;,
 4;48,42,34,40;,
 4;50,49,41,42;,
 4;49,51,43,41;,
 4;51,52,44,43;,
 4;52,53,45,44;,
 4;53,54,46,45;,
 4;54,55,47,46;,
 4;55,56,48,47;,
 4;56,50,42,48;,
 4;58,57,49,50;,
 4;57,59,51,49;,
 4;59,60,52,51;,
 4;60,61,53,52;,
 4;61,62,54,53;,
 4;62,63,55,54;,
 4;63,64,56,55;,
 4;64,58,50,56;,
 4;66,65,57,58;,
 4;65,67,59,57;,
 4;67,68,60,59;,
 4;68,69,61,60;,
 4;69,70,62,61;,
 4;70,71,63,62;,
 4;71,72,64,63;,
 4;72,66,58,64;,
 4;74,73,65,66;,
 4;73,75,67,65;,
 4;75,76,68,67;,
 4;76,77,69,68;,
 4;77,78,70,69;,
 4;78,79,71,70;,
 4;79,80,72,71;,
 4;80,74,66,72;,
 3;81,73,74;,
 3;81,75,73;,
 3;81,76,75;,
 3;81,77,76;,
 3;81,78,77;,
 3;81,79,78;,
 3;81,80,79;,
 3;81,74,80;;
 
 MeshMaterialList {
  4;
  88;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.037600;0.000000;1.000000;;
   5.000000;
   0.600000;0.600000;0.600000;;
   0.310000;0.014570;0.000000;;
  }
  Material {
   0.269600;0.172800;0.047200;1.000000;;
   7.000000;
   0.790000;0.790000;0.790000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.012800;0.000000;0.220000;1.000000;;
   10.000000;
   0.520000;0.520000;0.520000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  85;
  0.000000;-1.000000;0.000000;,
  -0.000000;-0.971128;-0.238559;,
  0.168692;-0.971127;-0.168689;,
  0.238564;-0.971127;-0.000000;,
  0.168690;-0.971128;0.168686;,
  -0.000000;-0.971130;0.238551;,
  -0.168691;-0.971127;0.168687;,
  -0.238566;-0.971126;-0.000000;,
  -0.168693;-0.971126;-0.168691;,
  -0.000000;-0.720778;-0.693166;,
  0.490145;-0.720777;-0.490142;,
  0.693169;-0.720775;0.000000;,
  0.490146;-0.720777;0.490141;,
  0.000000;-0.720781;0.693163;,
  -0.490145;-0.720777;0.490141;,
  -0.693168;-0.720776;0.000000;,
  -0.490144;-0.720777;-0.490143;,
  0.000000;-0.371341;-0.928496;,
  0.656546;-0.371343;-0.656545;,
  0.928495;-0.371345;-0.000000;,
  0.656547;-0.371341;0.656546;,
  0.000000;-0.371337;0.928498;,
  -0.656545;-0.371341;0.656547;,
  -0.928494;-0.371346;-0.000000;,
  -0.656545;-0.371344;-0.656546;,
  0.000309;-0.158872;-0.987299;,
  0.697381;-0.158693;-0.698911;,
  0.986676;-0.162698;-0.000743;,
  0.697173;-0.162577;0.698226;,
  0.000000;-0.159974;0.987121;,
  -0.697999;-0.159976;0.698001;,
  -0.987120;-0.159979;-0.000000;,
  -0.697999;-0.159975;-0.698001;,
  0.000868;-0.036221;-0.999343;,
  0.704382;-0.036077;-0.708904;,
  0.999195;-0.040075;-0.002077;,
  0.705065;-0.040000;0.708014;,
  0.000001;-0.037345;0.999303;,
  -0.706612;-0.037348;0.706615;,
  -0.999302;-0.037351;-0.000000;,
  -0.706612;-0.037348;-0.706615;,
  0.000868;0.036222;-0.999343;,
  0.704382;0.036077;-0.708904;,
  0.999195;0.040074;-0.002077;,
  0.705064;0.040001;0.708014;,
  0.000001;0.037346;0.999302;,
  -0.706612;0.037349;0.706615;,
  -0.999302;0.037351;0.000000;,
  -0.706612;0.037348;-0.706615;,
  0.000309;0.158870;-0.987300;,
  0.697381;0.158691;-0.698912;,
  0.986676;0.162695;-0.000742;,
  0.697173;0.162574;0.698226;,
  0.000000;0.159969;0.987122;,
  -0.697999;0.159973;0.698001;,
  -0.987121;0.159976;0.000000;,
  -0.697999;0.159972;-0.698001;,
  0.000000;0.371345;-0.928495;,
  0.656545;0.371346;-0.656544;,
  0.928494;0.371348;0.000000;,
  0.656546;0.371345;0.656544;,
  0.000000;0.371344;0.928495;,
  -0.656544;0.371346;0.656545;,
  -0.928493;0.371349;0.000000;,
  -0.656544;0.371347;-0.656545;,
  0.000000;0.720781;-0.693163;,
  0.490143;0.720779;-0.490140;,
  0.693166;0.720778;0.000000;,
  0.490143;0.720779;0.490140;,
  0.000000;0.720781;0.693163;,
  -0.490142;0.720779;0.490141;,
  -0.693165;0.720779;0.000000;,
  -0.490142;0.720780;-0.490140;,
  0.000000;0.638964;-0.769237;,
  0.543934;0.638962;-0.543933;,
  0.769239;0.638961;-0.000000;,
  0.543934;0.638962;0.543933;,
  0.000000;0.638964;0.769237;,
  -0.543935;0.638960;0.543934;,
  -0.769241;0.638959;-0.000000;,
  -0.543935;0.638960;-0.543934;,
  0.679901;0.274718;-0.679901;,
  0.679900;0.274718;0.679901;,
  -0.679900;0.274719;0.679901;,
  -0.679900;0.274719;-0.679901;;
  88;
  3;1,2,0;,
  3;2,3,0;,
  3;3,4,0;,
  3;4,5,0;,
  3;5,6,0;,
  3;6,7,0;,
  3;7,8,0;,
  3;8,1,0;,
  4;9,10,2,1;,
  4;10,11,3,2;,
  4;11,12,4,3;,
  4;12,13,5,4;,
  4;13,14,6,5;,
  4;14,15,7,6;,
  4;15,16,8,7;,
  4;16,9,1,8;,
  4;17,18,10,9;,
  4;18,19,11,10;,
  4;19,20,12,11;,
  4;20,21,13,12;,
  4;21,22,14,13;,
  4;22,23,15,14;,
  4;23,24,16,15;,
  4;24,17,9,16;,
  4;25,26,18,17;,
  4;26,27,19,18;,
  4;27,28,20,19;,
  4;28,29,21,20;,
  4;29,30,22,21;,
  4;30,31,23,22;,
  4;31,32,24,23;,
  4;32,25,17,24;,
  4;33,34,26,25;,
  4;34,35,27,26;,
  4;35,36,28,27;,
  4;36,37,29,28;,
  4;37,38,30,29;,
  4;38,39,31,30;,
  4;39,40,32,31;,
  4;40,33,25,32;,
  4;41,42,34,33;,
  4;42,43,35,34;,
  4;43,44,36,35;,
  4;44,45,37,36;,
  4;45,46,38,37;,
  4;46,47,39,38;,
  4;47,48,40,39;,
  4;48,41,33,40;,
  4;49,50,42,41;,
  4;50,51,43,42;,
  4;51,52,44,43;,
  4;52,53,45,44;,
  4;53,54,46,45;,
  4;54,55,47,46;,
  4;55,56,48,47;,
  4;56,49,41,48;,
  4;57,58,50,49;,
  4;58,59,51,50;,
  4;59,60,52,51;,
  4;60,61,53,52;,
  4;61,62,54,53;,
  4;62,63,55,54;,
  4;63,64,56,55;,
  4;64,57,49,56;,
  4;65,66,58,57;,
  4;66,67,59,58;,
  4;67,68,60,59;,
  4;68,69,61,60;,
  4;69,70,62,61;,
  4;70,71,63,62;,
  4;71,72,64,63;,
  4;72,65,57,64;,
  4;73,74,66,65;,
  4;74,75,67,66;,
  4;75,76,68,67;,
  4;76,77,69,68;,
  4;77,78,70,69;,
  4;78,79,71,70;,
  4;79,80,72,71;,
  4;80,73,65,72;,
  3;81,74,73;,
  3;81,75,74;,
  3;82,76,75;,
  3;82,77,76;,
  3;83,78,77;,
  3;83,79,78;,
  3;84,80,79;,
  3;84,73,80;;
 }
 MeshTextureCoords {
  82;
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