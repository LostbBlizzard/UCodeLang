

|Main[] -> byte:



 //if
 ~[1 == 8]:


  <<-;//jump out.
 ^[1 == 8]://if else
  
  <<-;

 //else
 ^:
  
  <<-; //jump out.
 
 //if else
 ~[true]:
 


  ~ <-;

 
 //Do Loop.
 ~:
  


  ~[true]: ~ <-;


 SomeJump:
  
  SomeJump <-;