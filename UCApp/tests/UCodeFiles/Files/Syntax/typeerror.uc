
|coolmultypes[] -> int||bool;
|coolmultypes3[] -> int||bool||char;
|coolmultypes4[] -> bool||char||int;

|main[] => type(bind(typeof(coolmultypes3())) == bind(typeof(coolmultypes4())));
