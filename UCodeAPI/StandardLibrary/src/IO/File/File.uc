


$OpenedFile;

$OpenFileError;

$File:
 |Open[PathSpan path] -> OpenedFile!OpenFileError;
 |Open[imut PathString& path] -> OpenedFile!OpenFileError;

 |GetString[PathSpan path] -> String!OpenFileError;
 |GetString[imut PathString& path] ->  String!OpenFileError;

 |GetBytes[PathSpan path] -> byte[]!OpenFileError;
 |GetBytes[imut PathString& path] ->  byte[]!OpenFileError;
