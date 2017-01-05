
-- Lua 에 추가되지 않은 A 구조체값을 a라는 변수에 넣는다.
a = Ctest:get()

-- temp 에 새 test 객체를 만들어 넣는다.
temp = test(4)

-- 임의의 객체 a를 Lua->C++로 전달한다.
temp:set(a)
-- test::set(A a) 함수 호출로 _test 에 생긴 변화를 확인한다.
print(temp._test)

-- 상속받은 부모의 함수 호출
print(temp:is_base())

-- 자기 자신의 함수 호출
print(temp:is_test())



-- Lua 에서 객체는 userdata로 인식된다.
print("g_test	-> ", g_test)
print("temp	-> ", temp)
print("a	-> ", a)



-- constructor 를 통해 생성한 temp 객체 정보를 살펴본다.
print("objinfo(temp)")
objinfo(temp)

-- 등록하지 않은 A 구조체 형의 a 객체 정보를 살펴본다.
print("objinfo(a)")
objinfo(a)

