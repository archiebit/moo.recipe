#ifndef MOO_TOKEN_HH
#define MOO_TOKEN_HH

// Project.
#include <moo/lexeme.hh>




namespace moo
{
    enum token_type
    {
        // Token categories.
        SPECIAL = 0x0001'0000,
        KEYWORD = 0x0002'0000,
        LITERAL = 0x0003'0000,
        COMMAND = 0x0004'0000,
        ONECHAR = 0x0005'0000,

        NONE    = 0x0001 | SPECIAL,
        LINE    = 0x0002 | SPECIAL,
        COMMENT = 0x0003 | SPECIAL,

        PREPARE = 0x0001 | KEYWORD,
        CONFIG  = 0x0002 | KEYWORD,
        RECIPE  = 0x0003 | KEYWORD,

        ID      = 0x0001 | LITERAL,
        STRING  = 0x0002 | LITERAL,

        ASSIGN  = 0x0001 | COMMAND,
        SAME    = 0x0002 | COMMAND,
        DIFF    = 0x0003 | COMMAND,

        SQUARE  = 0x0001 | ONECHAR
    };


    bool operator == ( token_type a, token_type b );
    bool operator != ( token_type a, token_type b );
}


namespace moo
{
    class token
    {
    public:
       ~token( );
        token( );
        token( token_type option, std::size_t line, std::size_t column, std::size_t length );


        friend bool operator == ( token const & value, std::u32string_view text );
        friend bool operator != ( token const & value, std::u32string_view text );

        friend bool operator == ( token const & value, token_type option );
        friend bool operator != ( token const & value, token_type option );


        void rewrite( token_type option );
        void rewrite( std::size_t line, std::size_t column, std::size_t length );


        operator token_type const &( ) const;
        operator     lexeme const &( ) const;

        operator token_type &( );
        operator     lexeme &( );


    private:
        token_type option;
        lexeme     source;
    };
}




#endif