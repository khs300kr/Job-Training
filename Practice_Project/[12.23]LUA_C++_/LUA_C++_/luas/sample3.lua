
-- Lua �� �߰����� ���� A ����ü���� a��� ������ �ִ´�.
a = Ctest:get()

-- temp �� �� test ��ü�� ����� �ִ´�.
temp = test(4)

-- ������ ��ü a�� Lua->C++�� �����Ѵ�.
temp:set(a)
-- test::set(A a) �Լ� ȣ��� _test �� ���� ��ȭ�� Ȯ���Ѵ�.
print(temp._test)

-- ��ӹ��� �θ��� �Լ� ȣ��
print(temp:is_base())

-- �ڱ� �ڽ��� �Լ� ȣ��
print(temp:is_test())



-- Lua ���� ��ü�� userdata�� �νĵȴ�.
print("g_test	-> ", g_test)
print("temp	-> ", temp)
print("a	-> ", a)



-- constructor �� ���� ������ temp ��ü ������ ���캻��.
print("objinfo(temp)")
objinfo(temp)

-- ������� ���� A ����ü ���� a ��ü ������ ���캻��.
print("objinfo(a)")
objinfo(a)

