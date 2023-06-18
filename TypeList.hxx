#pragma once

template< typename T, typename... Ts >
struct Front_
{
    using Type = T;
};
template< template< typename... Us > typename TL, typename T, typename... Ts >
struct Tail_
{
    using Type = TL< Ts... >;
};

template< unsigned N, typename T, typename... Ts >
struct Back_
{
    using Type = typename Back_< N - 1, Ts... >::Type;
};
template< typename T, typename... Ts >
struct Back_< 1, T, Ts... >
{
    using Type = T;
};

constexpr size_t TLNPos = std::numeric_limits< size_t >::max ();
template< size_t I, typename U, typename T, typename... Ts >
struct IndexOf_
{
    static constexpr size_t Value = std::is_same_v< U, T > //
                                        ? I
                                        : IndexOf_<  //
                                              I + 1, //
                                              U,     //
                                              Ts...  //
                                              >::Value;

    static_assert ( I > 1 || Value > 0, "Not in list" );
    static_assert ( I > 0, "Index 0 is reserved" );
};
template< size_t I, typename U, typename T >
struct IndexOf_< I, U, T >
{
    static constexpr size_t Value = std::is_same_v< U, T > ? I : 0;
};

template< typename... Ts >
struct TypeList
{
    using Front = typename Front_< Ts... >::Type;
    using Tail  = typename Tail_< TypeList, Ts... >::Type;
    using Back  = typename Back_< sizeof...( Ts ), Ts... >::Type;

    template< typename... Us >
    using Append = TypeList< Ts..., Us... >;
    template< typename... Us >
    using AppendFront = TypeList< Us..., Ts... >;

    template< template< typename... Us > typename T >
    using Transmute = T< Ts... >;

    template< typename T >
    static constexpr const bool   Has  = ( std::is_same_v< T, Ts > || ... );
    static constexpr const size_t Size = sizeof...( Ts );

    template< typename U >
    static constexpr const size_t IndexOf = IndexOf_< 1, U, Ts... >::Value - 1;

    constexpr static void ForEach ( auto&& functor )
    {
        ( functor.template operator()< Ts > (), ... );
    }
    constexpr static void ForEachIndexed ( auto&& functor )
    {
        ForEachIndexed_< 0, Ts... > ( functor );
    }

    private:

    template< size_t I, typename U, typename... Us >
    constexpr static void ForEachIndexed_ ( auto&& functor )
    {
        functor.template operator()< I, U > ();

        if constexpr ( sizeof...( Us ) > 0 )
            ForEachIndexed_< I + 1, Us... > ( functor );
    }
};
