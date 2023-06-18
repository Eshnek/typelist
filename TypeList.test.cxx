#include "TypeList.hxx"

template< typename... Ts >
struct TypeList_
{
};

CASE ( "TypeList" )
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

    SECTION ( "Front" )
    {
        static_assert ( Same< TL::Front, A > );
    }
    SECTION ( "Tail" )
    {
        static_assert ( Same< TL::Tail, TypeList< B, C > > );
    }
    SECTION ( "Back" )
    {
        static_assert ( Same< TL::Back, C > );
    }

    SECTION ( "Append" )
    {
        static_assert ( Same< TL::Append< char >, TypeList< A, B, C, char > > );
    }
    SECTION ( "AppendFront" )
    {
        static_assert ( Same< TL::AppendFront< char >, TypeList< char, A, B, C > > );
    }

    SECTION ( "Transmute" )
    {
        static_assert ( Same< TL::Transmute< TypeList >, TL > );
        static_assert ( Same< TL::Transmute< TypeList_ >, TypeList_< A, B, C > > );
    }

    SECTION ( "Has" )
    {
        static_assert ( TL::Has< A > );
        static_assert ( TL::Has< B > );
        static_assert ( TL::Has< C > );

        static_assert ( !TL::Has< double > );
        static_assert ( !TL::Has< float > );
    }
    SECTION ( "Size" )
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
            same = Same< T, A >;
            break;
        case 1:
            same = Same< T, B >;
            break;
        case 2:
            same = Same< T, C >;
            break;
        }

        REQUIRE ( same );
    };

    SECTION ( "ForEach" )
    {
        size_t idx {};

        TL::ForEach (
            [ &checkSame, &idx ]< typename T >
            {
                checkSame.operator()< T > ( idx++ );
            }
        );

        REQUIRE ( idx == 3 );
    }
    SECTION ( "ForEachIndexed" )
    {
        Vector< size_t > indexes;

        TL::ForEachIndexed (
            [ &checkSame, &indexes ]< size_t I, typename T >
            {
                checkSame.operator()< T > ( I );
                indexes.emplace_back ( I );
            }
        );

        REQUIRE ( indexes.size () == 3 );
        REQUIRE ( indexes[ 0 ] == 0 );
        REQUIRE ( indexes[ 1 ] == 1 );
        REQUIRE ( indexes[ 2 ] == 2 );
    }

    SECTION ( "IndexOf" )
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
