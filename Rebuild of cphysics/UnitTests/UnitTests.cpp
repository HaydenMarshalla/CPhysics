#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Cphysics_UnitTests
{
	TEST_CLASS(Vectors2DTest)
	{
	public:
		TEST_METHOD(deault_constructor)
		{
			Vectors2D vec = Vectors2D();
			Assert::AreEqual(0.0f, vec.x, 0.0f);
			Assert::AreEqual(0.0f, vec.y, 0.0f);
		}
		TEST_METHOD(copy_constructor)
		{
			Vectors2D vec = Vectors2D(4.0f, 2.0f);
			Vectors2D v = Vectors2D(vec);

			Assert::AreEqual(4.0f, v.x, 0.0f);
			Assert::AreEqual(2.0f, v.y, 0.0f);

			vec += Vectors2D(2.0f, 2.0f);

			Assert::AreNotEqual(4.0f, vec.x, 0.0f);
			Assert::AreNotEqual(2.0f, vec.y, 0.0f);

			Assert::AreEqual(6.0f, vec.x, 0.0f);
			Assert::AreEqual(4.0f, vec.y, 0.0f);
		}
		TEST_METHOD(coordinate_constructor)
		{
			Vectors2D vec = Vectors2D(-4.0f, 123.0f);
			Assert::AreEqual(-4.0f, vec.x, 0.0f);
			Assert::AreEqual(123.0f, vec.y, 0.0f);
		}
		TEST_METHOD(direction_constructor)
		{
			Vectors2D vec = Vectors2D(2.0f);
			Assert::AreEqual(-0.4161468365471424f, vec.x, 0.0f);
			Assert::AreEqual(0.9092974268256817f, vec.y, 0.0f);
		}
		TEST_METHOD(set)
		{
			Vectors2D vec = Vectors2D(4.0f, 2.0f);
			vec.set(33.0f, 7.0f);

			Assert::AreEqual(33.0f, vec.x, 0.0f);
			Assert::AreEqual(7.0f, vec.y, 0.0f);
		}
		TEST_METHOD(set_zero)
		{
			Vectors2D vec = Vectors2D(234.0f, -21323.0f);
			vec.setZero();

			Assert::AreEqual(0.0f, vec.x, 0.0f);
			Assert::AreEqual(0.0f, vec.y, 0.0f);
		}
		TEST_METHOD(validity_check)
		{
			Vectors2D validVec = Vectors2D(maxf, -maxf);
			Assert::IsTrue(validVec.isValid());

			Vectors2D nanVec1 = Vectors2D(NAN, -21323.0f);
			Assert::IsFalse(nanVec1.isValid());

			Vectors2D nanVec2 = Vectors2D(-maxf, NAN);
			Assert::IsFalse(nanVec2.isValid());

			float ininity = std::numeric_limits<float>::infinity();

			Vectors2D iniityVec = Vectors2D(ininity, ininity);
			Assert::IsFalse(iniityVec.isValid());
		}
		TEST_METHOD(negate_vector)
		{
			Vectors2D vec = Vectors2D(234.0f, -21323.0f);
			vec = -vec;
			Assert::AreEqual(-234.0f, vec.x, 0.0f);
			Assert::AreEqual(21323.0f, vec.y, 0.0f);
		}
		TEST_METHOD(add_vec)
		{
			Vectors2D vec1 = Vectors2D(2.0f, 3.0f);
			Vectors2D vec2 = Vectors2D(-1.2f, -5.4f);
			vec1 += vec2;
			Assert::AreEqual(0.8f, vec1.x, EPSILON);
			Assert::AreEqual(-2.4f, vec1.y, EPSILON);
		}
		TEST_METHOD(scalar)
		{
			Vectors2D vec1 = Vectors2D(2.0f, 3.0f);
			vec1 *= 2.5;
			Assert::AreEqual(5.0f, vec1.x, 0.0f);
			Assert::AreEqual(7.5f, vec1.y, 0.0f);
		}
		TEST_METHOD(sub_vec)
		{
			Vectors2D vec1 = Vectors2D(2.0f, 3.0f);
			vec1 -= Vectors2D(2.0f, 3.0f);
			Assert::AreEqual(0.0f, vec1.x, 0.0f);
			Assert::AreEqual(0.0f, vec1.y, 0.0f);
		}
		TEST_METHOD(len)
		{
			Vectors2D vec1 = Vectors2D(2.0f, 3.0f);
			float len = vec1.len();
			Assert::AreEqual(3.605551275463989f, len, EPSILON);
		}
		TEST_METHOD(lengthSqr)
		{
			Vectors2D vec1 = Vectors2D(2.0f, 3.0f);
			float len = vec1.lengthSqr();
			Assert::AreEqual(13.0f, len, 0.0f);
		}
		TEST_METHOD(normalize)
		{
			Vectors2D vec1 = Vectors2D(2.0f, 3.0f);
			float len = vec1.Normalize();
			Assert::AreEqual(3.605551275463989f, len, EPSILON);
			Assert::AreEqual(0.5547001962252291f, vec1.x, EPSILON);
			Assert::AreEqual(0.8320502943378437f, vec1.y, EPSILON);
		}
		TEST_METHOD(normal)
		{
			Vectors2D vec1 = Vectors2D(0.0f, 1.0f);
			Vectors2D vec2 = vec1.normal();
			Assert::AreEqual(-1.0f, vec2.x, 0.0f);
			Assert::AreEqual(0.0f, vec2.y, 0.0f);
		}
		TEST_METHOD(scalar_inline)
		{
			Vectors2D vec1 = Vectors2D(5.0f, 2.0f);
			Vectors2D vec2 = vec1 * 4.0f;
			Assert::AreEqual(5.0f, vec1.x, 0.0f);
			Assert::AreEqual(2.0f, vec1.y, 0.0f);
			Assert::AreEqual(20.0f, vec2.x, 0.0f);
			Assert::AreEqual(8.0f, vec2.y, 0.0f);

			Vectors2D vec3 = 4.0f * vec1;
			Assert::AreEqual(20.0f, vec3.x, 0.0f);
			Assert::AreEqual(8.0f, vec3.y, 0.0f);
		}
		TEST_METHOD(add_inline)
		{
			Vectors2D vec1 = Vectors2D(0.0f, 1.0f);
			Vectors2D vec2 = Vectors2D(4.7f, 1.0f);
			Vectors2D vec3 = vec1 + vec2;
			Assert::AreEqual(4.7f, vec3.x, 0.0f);
			Assert::AreEqual(2.0f, vec3.y, 0.0f);
		}
		TEST_METHOD(subtract_inline)
		{
			Vectors2D vec1 = Vectors2D(0.0f, 1.0f);
			Vectors2D vec2 = Vectors2D(4.7f, 1.0f);
			Vectors2D vec3 = vec1 - vec2;
			Assert::AreEqual(-4.7f, vec3.x, 0.0f);
			Assert::AreEqual(0.0f, vec3.y, 0.0f);
		}
		TEST_METHOD(equals)
		{
			Vectors2D vec1 = Vectors2D(0.0f, 1.0f);
			Vectors2D vec2 = Vectors2D(4.7f, 1.0f);
			Assert::IsFalse(vec1 == vec2);
			vec1 = Vectors2D(1.0f, 1.0f);
			vec2 = Vectors2D(1.0f, 1.0f);
			Assert::IsTrue(vec1 == vec2);
		}
		TEST_METHOD(not_equals)
		{
			Vectors2D vec1 = Vectors2D(0.0f, 1.0f);
			Vectors2D vec2 = Vectors2D(4.7f, 1.0f);
			Assert::IsTrue(vec1 != vec2);
			vec1 = Vectors2D(1.0f, 1.0f);
			vec2 = Vectors2D(1.0f, 1.0f);
			Assert::IsFalse(vec1 != vec2);
		}
		TEST_METHOD(cross_product)
		{
			Vectors2D vec1 = Vectors2D(5.03f, 1.30f);
			Vectors2D w = crossProduct(vec1, 6.0f);
			Assert::AreEqual(-7.8f, w.x, 0.01f);
			Assert::AreEqual(30.18f, w.y, 0.01f);

			vec1 = Vectors2D(-3.75f, 9.34f);
			w = crossProduct(vec1, 1003.4f);
			Assert::AreEqual(-9371.756f, w.x, 0.01f);
			Assert::AreEqual(-3762.75f, w.y, 0.01f);

			vec1 = Vectors2D(5.03f, 1.30f);
			w = crossProduct(6.0f, vec1);
			Assert::AreEqual(-7.8f, w.x, 0.01f);
			Assert::AreEqual(30.18f, w.y, 0.01f);

			vec1 = Vectors2D(-3.75f, 9.34f);
			w = crossProduct(1003.4f, vec1);
			Assert::AreEqual(-9371.756f, w.x, 0.01f);
			Assert::AreEqual(-3762.75f, w.y, 0.01f);
		}
		TEST_METHOD(dot_product)
		{
			Vectors2D vec1 = Vectors2D(5.0f, 1.0f);
			Vectors2D vec2 = Vectors2D(15.0f, 10.0f);
			float i = dotProduct(vec1, vec2);
			Assert::AreEqual(85.0f, i, 0.0f);
		}
		TEST_METHOD(dist)
		{
			Vectors2D vec1 = Vectors2D(5.0f, 2.0f);
			Vectors2D vec2 = Vectors2D(7.0f, 1.0f);
			float seperation = distance(vec1, vec2);
			Assert::AreEqual(2.2361f, seperation, EPSILON);
		}
	};

	TEST_CLASS(Matrix2DTest)
	{
	public:
		TEST_METHOD(default_constructor)
		{
			Matrix2D u = Matrix2D();
			Assert::AreEqual(0.0f, u.row1.x, 0.0f);
			Assert::AreEqual(0.0f, u.row1.y, 0.0f);
			Assert::AreEqual(0.0f, u.row2.x, 0.0f);
			Assert::AreEqual(0.0f, u.row2.y, 0.0f);
		}
		TEST_METHOD(radians_constructor)
		{
			Matrix2D u = Matrix2D(1.1f);
			Assert::AreEqual(0.45359612142f, u.row1.x, EPSILON);
			Assert::AreEqual(-0.89120736006f, u.row1.y, EPSILON);
			Assert::AreEqual(0.89120736006f, u.row2.x, EPSILON);
			Assert::AreEqual(0.45359612142f, u.row2.y, EPSILON);
		}
		TEST_METHOD(set)
		{
			Matrix2D u = Matrix2D();
			u.set(5.0f);
			Assert::AreEqual(0.28366218546f, u.row1.x, EPSILON);
			Assert::AreEqual(0.95892427466f, u.row1.y, EPSILON);
			Assert::AreEqual(-0.95892427466f, u.row2.x, EPSILON);
			Assert::AreEqual(0.28366218546f, u.row2.y, EPSILON);
		}
		TEST_METHOD(transpose)
		{
			Matrix2D u = Matrix2D(1.0f);
			Matrix2D r = u.Transpose();
			Assert::AreEqual(0.54030230586f, r.row1.x, EPSILON);
			Assert::AreEqual(0.8414709848f, r.row1.y, EPSILON);
			Assert::AreEqual(-0.8414709848f, r.row2.x, EPSILON);
			Assert::AreEqual(0.54030230586f, r.row2.y, EPSILON);

			Assert::AreEqual(0.54030230586f, u.row1.x, EPSILON);
			Assert::AreEqual(-0.8414709848f, u.row1.y, EPSILON);
			Assert::AreEqual(0.8414709848f, u.row2.x, EPSILON);
			Assert::AreEqual(0.54030230586f, u.row2.y, EPSILON);
		}
		TEST_METHOD(muliply_operator)
		{
			Matrix2D u = Matrix2D(1.0f);
			Vectors2D v = Vectors2D(1.0f, 0.0f);
			Vectors2D vcopy = u * v;
			Assert::AreEqual(1.0f, v.x, EPSILON);
			Assert::AreEqual(0.0f, v.y, 0.0f);
		}
		TEST_METHOD(multiply_inline)
		{
			Matrix2D m = Matrix2D();
			m.set(1.0f);
			Vectors2D v = Vectors2D(1.0f, 0.0f);
			mul(m, v);
			Assert::AreEqual(v.x, 0.5403023058681398f, EPSILON);
			Assert::AreEqual(v.y, 0.8414709848078965f, EPSILON);
		}
		TEST_METHOD(multiply_inline_out)
		{
			Matrix2D m = Matrix2D();
			m.set(1.0f);
			Vectors2D v = Vectors2D(1.0f, 0.0f);
			Vectors2D q = Vectors2D(10.0f, 0.0f);
			q = mul(m, v, q);
			Assert::AreEqual(q.x, 0.5403023058681398f, EPSILON);
			Assert::AreEqual(q.y, 0.8414709848078965f, EPSILON);
			Assert::AreEqual(v.x, 1.0f, EPSILON);
			Assert::AreEqual(v.y, 0.0f, 0.0f);
		}
	};
}
