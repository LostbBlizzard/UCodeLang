
|coolmultypes[] -> int||bool;
|coolmultypes3[] -> int||bool||char;
|coolmultypes4[] -> bool||char||int;

|main[] => typeof(coolmultypes()) == typeof(coolmultypes4());