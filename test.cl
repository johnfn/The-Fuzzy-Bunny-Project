class Main inherits IOplus {
  main() : Object {{
    newline (1);
    out_string_ln ("---Code Generation Correctness Test---");
    newline (1);

    let missed : Int in {
      missed <- missed + (new ArithTest1).test();
      missed <- missed + (new ArithTest2).test();
      missed <- missed + (new ComparisonTest).test();
      missed <- missed + (new LoopTest).test();
      missed <- missed + (new ConditionTest).test();
      missed <- missed + (new StringTest).test();
      missed <- missed + (new SideEffectTest).test();
      missed <- missed + (new LetTest).test();
      missed <- missed + (new CaseTest).test();
      missed <- missed + (new IsVoidTest).test();
      missed <- missed + (new SelfTypeTest).test();

      newline (1);
      if missed = 0 then
        out_string_ln ("Perfect! Nice job.")
      else
        out_string_ln ("Go fix the bugs.")
      fi;
      newline (1);
    };
  }};
};

class IOplus inherits IO {
  out_string_ln (str : String) : SELF_TYPE {{
    out_string (str);
    out_string ("\n");
  }};

  newline (n : Int) : Object {
    while 0 < n loop {
      out_string ("\n");
      n <- n - 1;
    } pool
  };
};

class Judge inherits IOplus {
  judge (a : Object, b : Object) : Bool { 
    if a = b then {
      out_string_ln ("correct");
      true;
    } else {
      out_string_ln ("incorrect");
      false;
    } fi 
  };
};

class Problem inherits Judge {
  test() : Int {{
    out_string (name());
    out_string_ln (":");
    out_string ("\t");
    if judge (solve(), answer()) then 0 else 1 fi;
  }};

  name() : String { "unknown" };
  solve() : Object { out_string_ln ("solve not implemented") };
  answer() : Object { out_string_ln ("answer not implemented") };
};

class ArithTest1 inherits Problem {
  name() : String { "ArithTest1" };
  solve() : Object { 10 + (1 - 2 + 3 * 2) / 5 + ~1  };
  answer() : Object { 10 };
};

class ArithTest2 inherits Problem {
  name() : String { "ArithTest2" };

  a : Int <- 3 - 1; -- 2
  b : Int <- 4 + 1; -- 5

  solve() : Object {{
    a <- a * 2;     -- 4
    b <- a * a * b; -- 80



    let x : Int <- 2 in {
      b <- b / x / x; -- 20
      a <- ~b;        -- -20

      let a : Int <- 5 in {
        b <- b + a;   -- 25
      };


      a <- a + b;
    };
  }};

  answer() : Object { 5 };
};

class ComparisonTest inherits Problem {
  name() : String { "ComparisonTest" };
  
  a : Int <- 10 + b + c * ~1000;
  b : Int <- {
    if 9 <= a then 20 else 10 fi;  -- 20
  };
  c : Int <- {
    if ~(a + b) < ~29 then a + b else a - b fi; -- 30
  };

  solve() : Object { 60 <= a + b + c };
  answer() : Object { true };
};

class LoopTest inherits Problem {
  name() : String { "LoopTest" };

  a : Int <- 10;
  b : Int;
  c : Int <- 2;

  solve() : Object {{
    while 0 < a loop {
      b <- b + a;
      a <- a - 1;

      while 0 < c loop {
        b <- b + 1;
        c <- c - 1;
      } pool;

      c <- 2;
    } pool;

    b;
  }};

  answer() : Object { 75 };
};

class ConditionTest inherits Problem {
  name() : String { "ConditionTest" };

  a : Bool <- if b = 0 then false else true fi; -- false
  b : Int <- if not a then 10 else 20 fi;       -- 10

  solve() : Object {{
    if 1 < b then
      if 2 < b then
        if 3 < b then
          if 4 < b then
            if 30 < b then
              10
            else
              if 9 < b then
                if 11 < b then
                  11
                else
                  if 10 <= b then 1 else 2 fi
                fi
              else 9 fi
            fi
          else 8 fi
        else 7 fi
      else 6 fi
    else 5 fi;
  }};

  answer() : Object { 1 };
};

class StringTest inherits Problem {
  name() : String { "StringTest" };

  a : String <- "abc";
  b : String;
  c : String <- "def";
  s : String <- a.concat(b).concat(c);

  solve() : Object {{
    let i : Int, a : String in {
      while i < s.length() loop {
        a <- a.concat(s.substr(i,1)).concat(s.substr(i,1));
        i <- i + 1;
      } pool;
      a;
    };
  }};

  answer() : Object { "aabbccddeeff" };
};

class SideEffectTest inherits Problem {
  name() : String { "SideEffectTest" };

  n : Int <- 1;
  
  effect1() : Int { n <- 1 + n };
  effect2() : Int { n <- 1 + n * 2 };
  effect3() : Int { n <- 1 + n * 3 };

  method(a : Int, b : Int, c : Int) : Object { n <- n + a + b + c };

  solve() : Object {{
    method(effect1(), effect2(), effect3());
    self@SideEffectTest.method(effect3(), effect2(), effect1());
    n;
  }};

  answer() : Object { 831 };
};

class LetTest inherits Problem {
  name() : String { "LetTest" };

  n : Int <- 1;
  a : Int;

  solve() : Object {{
    let n : Int <- n * 2 + 1, 
        n : Int <- n + 3, 
        n : Int <- n * 3 + 1, 
        n : Int <- n * 1 in {
      a <- let n : Int <- n - 3,
               n : Int <- a - 4 + n,
               n : Int <- n + 2 in {
             n;
           };
      a <- a + n;
    };
    a <- a + n;
  }};

  answer() : Object { 34 };
};

class A  inherits IO { method() : Int {1}; };
class B1 inherits A  { method() : Int {2}; };
class B2 inherits A  { method() : Int {3}; };
class C  inherits B1 { method() : Int { { out_string("B1!!!") ;4;} }; };
class D  inherits C  { method() : Int {5}; };

class CaseTest inherits Problem {
  name() : String { "CaseTest" };

  c : Object;
  d : Object;
  a : Int;

  solve() : Object {{
    c <- new C;
    out_string("Starting cases\n");
    a <- 0;
    out_int(a);
    out_string("\n");
    a <- case c of x : A => x.method();
                            x : B2 => x.method();
                            x : B1 => { out_int(x.method());
                                        x.method(); };
                  esac;
    out_int(a);
    out_string("\n");
    c <- new D;
    a <- case c of x : Object => 9;
                            x : C => x.method();
                            x : D => x.method() * 10;
                  esac;
    out_int(a);
    out_string("\n");

    c <- new Int;
    a <- 10 * a + case c of x : Object => 9;
                            x : A => x.method();
                            x : String => 8;
                  esac;
    out_int(a);
    out_string("\n");

    c <- new A;
    a <- 10 * a + case c of x : A => {
                              let a : Int in {
                                a <- let x : B1 <- new B1 in x.method();
                                a <- a + x.method();
                              };
                            };
                  esac;
    out_int(a);
    out_string("\n");
  }};

  answer() : Object { 4593 };
};

class IsVoidTest inherits Problem {
  name() : String { "IsVoidTest" };

  a : A;
  b : C <- new C;
  n : Int;

  solve() : Object {{
    n <- n + if isvoid a then 1 else 0 fi; 
    n <- n + if isvoid b then 10 else 0 fi;
    
    a <- new B1;
    n <- n + if isvoid a then 100 else 0 fi;

    let b : A in {
      n <- n + if isvoid b then 1000 else 0 fi;
    };

    n <- n + if isvoid self then 10000 else 0 fi;
    n <- n + if Isvoid while false loop 0 pool then 100000 else 0 fi;

    out_int(n);
n;
  }};

  answer() : Object { 1641 };
};

class X { 
  n : Int;
  get_new_self() : SELF_TYPE { new SELF_TYPE };
  get_self() : SELF_TYPE { self };
  method() : Int {1};   
  inc() : Int { n <- n + 1 };
  getn() : Int { n };
};
class Y inherits X { method() : Int {2}; };
class Z inherits X { method() : Int {3}; };

class SelfTypeTest inherits Problem {
  name() : String { "SelfTypeTest" };

  x1 : X <- new X;
  x2 : X <- new Y;
  x3 : X <- new Z;
  a : Int;

  solve() : Object {{
    x2.get_self().inc();
    a <- a + x2.getn();
    a <- a + 2 * x2.get_self().method();
    a <- 10 * a + x2.get_new_self().get_self().inc(); 
    a <- 10 * a + x2.getn();
    a <- 10 * a + x3.get_self().method();
    a <- 10 * a + x3.get_new_self().method();  --Gives 1, should give 3.
  }};

  answer() : Object { 51133 };
};
