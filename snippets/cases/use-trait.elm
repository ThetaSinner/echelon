trait MyTrait {
  function myTraitMethod(string, integer) -> string
}

type MyType {
  function myTypeMethod {
    // do something
  }
}

MyType has MyTrait {
  myTraitMethod {
    // do something traity.
  }
}
