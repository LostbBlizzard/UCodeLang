# For Type

The For Type allows for added new funcions on an existing type

```cpp

$DayType enum:
 DayTime,
 NightTime,
 Noon,
 MidNight

$for DayType:
 |IsDayTime[this&]:
  ret this == DayType::DayTime;

|main[]:
 DayType val = DayType::DayTime;

 ret val.IsDayTime();

```

For type  also allows for static funcions

```cpp

$DayType enum:
 DayTime,
 NightTime,
 Noon,
 MidNight

$for DayType:
 |RandomTime[] -> DayType;

|main[]:
 DayType val = DayType::RandomTime();

 ret val;

```


## Notes

the scope the added funcions can only be used if for type is in scope
