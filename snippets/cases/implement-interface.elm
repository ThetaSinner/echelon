interface MyInterface {
  add(integer x, integer y) -> integer
}

type MyType <- MyInterface {
  add(integer t, integer p) -> integer {
   t + p
  }
}
