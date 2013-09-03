setup() {}
void loop() { my_function();}
void my_function()
{ 
  static int MyVar1 = 0;
  int MyVar2 = 0;
  MyVar1++;
  MyVar2++;
}

