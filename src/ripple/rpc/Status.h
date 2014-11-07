//------------------------------------------------------------------------------
/*
    This file is part of rippled: https://github.com/ripple/rippled
    Copyright (c) 2012, 2013 Ripple Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#ifndef RIPPLE_STATUS_H
#define RIPPLE_STATUS_H

#include <ripple/protocol/TER.h>
#include <ripple/rpc/ErrorCodes.h>

namespace ripple {
namespace RPC {
namespace New {

/** Status represents the results of an operation that might fail.

    It wraps the legacy codes TER and error_code_i, providing both a uniform
    interface and a way to attach additional information to existing status
    returns.

    A Status can also be used to fill a Json::Value with a JSON-RPC 2.0
    error response:  see http://www.jsonrpc.org/specification#error_object
 */
struct Status
{
public:
    enum class Type {none, TER, error_code_i};
    using Code = int;
    using Strings = std::vector <std::string>;

    static const Code OK = 0;

    Status () = default;

    Status (Code code, Strings d = {})
            : type_ (Type::none), code_ (code), messages_ (std::move (d))
    {
    }

    Status (TER ter, Strings d = {})
            : type_ (Type::TER), code_ (ter), messages_ (std::move (d))
    {
    }

    Status (error_code_i e, Strings d = {})
            : type_ (Type::error_code_i), code_ (e), messages_ (std::move (d))
    {
    }

    /* Returns a representation of the integer status Code as a string.
       If the Status is OK, the result is an empty string.
    */
    std::string codeString () const;

    /** Fill a Json::Value. If the Status is OK, fillJson has no effect. */
    void fillJson(Json::Value&);

    /** Returns true if the Status is *not* OK. */
    operator bool() const
    {
        return code_ != OK;
    }

    /** Returns true if the Status is OK. */
    bool operator !() const
    {
        return ! bool (*this);
    }

    /** Returns the Status as a TER.
        This may only be called if type() == Type::TER. */
    TER toTER () const
    {
        assert (type_ == Type::TER);
        return TER (code_);
    }

    /** Returns the Status as an error_code_i.
        This may only be called if type() == Type::error_code_i. */
    error_code_i toErrorCode() const
    {
        assert (type_ == Type::error_code_i);
        return error_code_i (code_);
    }

    Strings const& messages() const
    {
        return messages_;
    }

    Type type() const
    {
        return type_;
    }

private:
    Type type_ = Type::none;
    Code code_ = OK;
    Strings messages_;
};

} // namespace New
} // namespace RPC
} // ripple

#endif
