


$OpenedFile export;

$OpenFileError export;

$File:
 export |Open[PathSpan path] -> OpenedFile!OpenFileError;
 export |Open[imut PathString& path] -> OpenedFile!OpenFileError;

 export |GetString[PathSpan path] -> String!OpenFileError;
 export |GetString[imut PathString& path] ->  String!OpenFileError;

 export |GetBytes[PathSpan path] -> byte[]!OpenFileError;
 export |GetBytes[imut PathString& path] ->  byte[]!OpenFileError;
