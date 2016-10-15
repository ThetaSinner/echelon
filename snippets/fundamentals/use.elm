// Given a third party package packman::usefulcode
// with modules X, Y and Z

// example 1
package thetasinner::testpackman

use packman::usefulcode

usefulcode::X::someMethod()

// example 2
package thetasinner::testpackmanx

use packman::usefulcode::X

X::someMethod()

// example 3
package thetasinner::testpackmanxsomemethod

use packman::usefulcode::X::someMethod

someMethod()

// example 4
package thetasinner::testpackmany

use packman~Y

Y::anotherMethod()

// example 5
package thetasinner::testpackmanz

use packman~Z::method1,method2

method1()
method2()
