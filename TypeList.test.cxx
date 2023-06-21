#include "TypeList.hxx"

#include <cassert>
#include <iostream>
#include <vector>

template< typename... Ts >
struct TypeList_
{
};

void runTests ();

int main ()
{
    runTests ();

    std::cout << "Tests passed!\n";

    return 0;
}

void runTests ()
{
    struct A
    {
    };
    struct B
    {
    };
    struct C
    {
    };
    struct D
    {
    };
    struct E
    {
    };
    struct F
    {
    };
    struct G
    {
    };
    struct H
    {
    };
    struct I
    {
    };
    struct J
    {
    };

    using TL = TypeList< A, B, C >;

    // Front
    {
        static_assert ( std::is_same_v< TL::Front, A > );
    }
    // Tail
    {
        static_assert ( std::is_same_v< TL::Tail, TypeList< B, C > > );
    }
    // Back
    {
        static_assert ( std::is_same_v< TL::Back, C > );
    }

    // Append
    {
        static_assert ( std::is_same_v< TL::Append< char >, TypeList< A, B, C, char > > );
    }
    // AppendFront
    {
        static_assert ( std::is_same_v< TL::AppendFront< char >, TypeList< char, A, B, C > > );
    }

    // Transmute
    {
        static_assert ( std::is_same_v< TL::Transmute< TypeList >, TL > );
        static_assert ( std::is_same_v< TL::Transmute< TypeList_ >, TypeList_< A, B, C > > );
    }

    // Has
    {
        static_assert ( TL::Has< A > );
        static_assert ( TL::Has< B > );
        static_assert ( TL::Has< C > );

        static_assert ( !TL::Has< double > );
        static_assert ( !TL::Has< float > );
    }
    // Size
    {
        static_assert ( TL::Size == 3 );

        static_assert ( TypeList< A >::Size == 1 );
        static_assert ( TypeList< A, B >::Size == 2 );
        static_assert ( TypeList< A, B, C >::Size == 3 );
        static_assert ( TypeList< A, B, C, D >::Size == 4 );
        static_assert ( TypeList< A, B, C, D, E >::Size == 5 );
    }

    const auto checkSame = []< typename T > ( const auto idx )
    {
        bool same {};

        switch ( idx )
        {
        case 0:
            same = std::is_same_v< T, A >;
            break;
        case 1:
            same = std::is_same_v< T, B >;
            break;
        case 2:
            same = std::is_same_v< T, C >;
            break;
        }

        assert ( same );
    };

    // ForEach
    {
        size_t idx {};

        TL::ForEach (
            [ &checkSame, &idx ]< typename T >
            {
                checkSame.operator()< T > ( idx++ );
            }
        );

        assert ( idx == 3 );
    }
    // ForEachIndexed
    {
        std::vector< size_t > indexes;

        TL::ForEachIndexed (
            [ &checkSame, &indexes ]< size_t I, typename T >
            {
                checkSame.operator()< T > ( I );
                indexes.emplace_back ( I );
            }
        );

        assert ( indexes.size () == 3 );
        assert ( indexes[ 0 ] == 0 );
        assert ( indexes[ 1 ] == 1 );
        assert ( indexes[ 2 ] == 2 );
    }

    // IndexOf
    {
        static_assert ( TL::IndexOf< A > == 0 );
        static_assert ( TL::IndexOf< B > == 1 );
        static_assert ( TL::IndexOf< C > == 2 );

        // Error: Not in list
        // core::ignore = TL::IndexOf< D >;

        // Error: Reserved
        // core::ignore = IndexOf_<0, C, A, B, C >::Value;
    }
}
