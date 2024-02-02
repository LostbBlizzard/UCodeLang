


|main[]:
 char V1 = 'A';
 utf8 V2 = 'A';
 utf16 V3 = 'A';
 utf32 V4 = 'A';

 ret sizeof(bind(typeof(V1))) + sizeof(bind(typeof(V2))) + sizeof(bind(typeof(V3))) + sizeof(bind(typeof(V4)));