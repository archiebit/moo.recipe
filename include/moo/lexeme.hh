#ifndef MOO_LEXEME_HH
#define MOO_LEXEME_HH

#include <cstddef>
#include <string_view>




namespace moo
{
    class lexeme
    {
    public:
       ~lexeme( );
        lexeme( std::size_t line, std::size_t column, std::size_t length );


        friend bool operator == ( lexeme const & a, lexeme const & b );
        friend bool operator != ( lexeme const & a, lexeme const & b );


        operator std::u32string_view( ) const;


    private:
        std::size_t   line;
        std::size_t column;
        std::size_t length;
    };
}




#endif