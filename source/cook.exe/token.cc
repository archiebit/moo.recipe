// Project.
#include <moo/token.hh>




namespace moo
{
    bool operator == ( token_type a, token_type b )
    {
        unsigned short group[ 2 ] { a & 0xFFFF'0000, b & 0xFFFF'0000 };
        unsigned short value[ 2 ] { a & 0x0000'FFFF, b & 0x0000'FFFF };


        if( group[ 0 ] != group[ 1 ] )
        {
            return false;
        }

        if( value[ 0 ] == 0 or value[ 1 ] == 0 )
        {
            return true;
        }


        return value[ 0 ] == value[ 1 ];
    }


    bool operator != ( token_type a, token_type b )
    {
        return not ( a == b );
    }
}


namespace moo
{
    token::~token( ) = default;


    token::token( )
    :   option( NONE )
    ,   source( 0, 0, 0 )
    { }


    token::token( token_type option, std::size_t line, std::size_t column, std::size_t length )
    :   option( option )
    ,   source( line, column, length )
    { }




    bool operator == ( token const & value, std::u32string_view text )
    {
        return static_cast<std::u32string_view>( value.source ) == text;
    }


    bool operator != ( token const & value, std::u32string_view text )
    {
        return static_cast<std::u32string_view>( value.source ) != text;
    }




    bool operator == ( token const & value, token_type option )
    {
        return value.option == option;
    }


    bool operator != ( token const & value, token_type option )
    {
        return value.option != option;
    }




    void token::rewrite( token_type option )
    {
        this->option = option;
    }


    void token::rewrite( std::size_t line, std::size_t column, std::size_t length )
    {
        this->source.rewrite( line, column, length );
    }




    token::operator token_type const &( ) const
    {
        return option;
    }


    token::operator     lexeme const &( ) const
    {
        return source;
    }


    token::operator token_type &( )
    {
        return option;
    }


    token::operator     lexeme &( )
    {
        return source;
    }
}