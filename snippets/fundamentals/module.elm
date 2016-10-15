module MyModuleOne {

integer x = 10;
y = x + 10;

}

module MyModuleTwo exposes [newMyType] {

type PrivateType {
  integer t
}

type MyTypeInMyModuleTwo {
  integer x
  PrivateType t

  function aMethod {

  }
}

function newMyType {
  // create and return a new instance of MyTypeInMyModuleTwo
}

}

module MyHugeModule {
  expose function m1 {

  }

  expose function m2 {

  }

  expose function m3 {

  }

  expose function m4 {

  }

  expose function m5 {

  }
}

module MyExtensibleModule {

integer t

}

extend module MyExtensibleModule [getT] {

function getT {
  t
}

}

final module MyInextensibleModule [getT] {
  // actually private
  integer t
  function getT {

  }
}
