// file 1
module A [doSomething] {

function doSomething {
}

}

// file 2
module B {

function checkSomething -> boolean {

}

function doSomething {
  if (checkSomething()) {
    A::doSomething()
  }
}

}
