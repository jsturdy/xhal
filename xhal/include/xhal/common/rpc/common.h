/*!
 * \file
 * \brief Contains the classes required for defining remotely callable RPC methods
 *
 * \author Laurent Pétré <lpetre@ulb.ac.be>
 */

#ifndef XHAL_RPC_COMMON_H
#define XHAL_RPC_COMMON_H

#include "xhal/rpc/compat.h"
#include "xhal/rpc/helper.h"

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "xhal/rpc/wiscRPCMsg.h" // move the header to "xhal/extern/wiscRPCMsg.h" ?

namespace xhal { namespace rpc {

    /*!
     * \brief Defines the templated RPC ABI version
     */
    static constexpr const char* abiVersion = "v1";

    /*!
     * \brief Class whose all remotely callable RPC method must inherit from
     *
     * The required inheritance is used as a compile time check so a developer
     * cannot remotely call a local function by mistake.
     */
    struct Method
    {
        /*!
         * \brief The operator call must be define <b>once and only once</b> per
         * RPC method.
         *
         * This templated operator declaration is only shown as an example and
         * emphasizes the need of defining it in child classes.
         *
         * \warnng The call operator \b must be defined as \c const.
         */
        template<typename R, typename... Args> R operator()(Args...) const;
    };

    /*!
     * \brief Base of the \c MessageSerializer and \c MessageDeserializer classes
     *
     * \c MessageBase provides the key index tracking functionnality which
     * is mandatory for serialization.
     */
    class MessageBase {

        /*!
         * \brief Index to the next free/unread key
         */
        uint32_t _keyIdx = 0;

      protected:

        /*
         * \brief Returns the next free/unread key
         */
        inline uint32_t dispenseKey() { return _keyIdx++; }

    };

    /*!
     * \brief This class serializes parameters into a \c wisc::RPCMsg
     */
    class MessageSerializer : public MessageBase
    {

    protected:

        wisc::RPCMsg *m_wiscMsg;

        /*!
         * \brief Serializes custom types if possible or else supresses implicit type conversions
         *
         * Every type not defined hereunder is taken care of by this templated function.
         * The function serves two purposes:
         *
         * 1. It delegates the serialization to a well-known function.
         * 2. It aims at enforcing maximum type compatibility with the UW RPC API by
         *    remembering the developer that she/he can transmit defined types over the
         *    network.
         */
        template<typename T> inline void save(const T &t) {
            // This const_cast is safe when the API is used as intented
            // More precisely when the object t is modified only with the operator&
            serialize(*this, const_cast<T &>(t));
        }

        /*!
         * \brief Adds a \c std::uint32_t to the message
         */
        inline void save(const std::uint32_t value) {
            m_wiscMsg->set_word(std::to_string(dispenseKey()), value);
        }

        /*!
         * \brief Adds a \c std::vector<std::uint32_t> to the message
         */
        inline void save(const std::vector<std::uint32_t> &value) {
            m_wiscMsg->set_word_array(std::to_string(dispenseKey()), value);
        }

        /*!
         * \brief Adds a \c std::string to the message
         */
        inline void save(const std::string &value) {
            m_wiscMsg->set_string(std::to_string(dispenseKey()), value);
        }

        /*!
         * \brief Adds a \c std::vector<std::string> to the message
         */
        inline void save(const std::vector<std::string> &value) {
            m_wiscMsg->set_string_array(std::to_string(dispenseKey()), value);
        }

        /*!
         * \brief Adds a \c std::array<T> to the message where \c T is an integral type (except \c bool)
         */
        template<typename T,
                 std::size_t N,
                 typename std::enable_if<std::is_integral<T>::value && !helper::is_bool<T>::value, int>::type = 0
                >
        inline void save(const std::array<T, N> &value) {
            m_wiscMsg->set_binarydata(std::to_string(dispenseKey()), value.data(), N*sizeof(T));
        }

        /*!
         * \brief Adds a \c std::map<std::uint32_t, T> to the message where \c T is a serializable type
         */
        template<typename T> inline void save(const std::map<std::uint32_t, T> &value) {
            // The first RPC key stores the std::map keys
            // This is required to know the std::map size at deserialization
            const auto keysKey = dispenseKey();

            std::vector<std::uint32_t> keys{};
            keys.reserve(value.size());

            for (const auto & elem : value) {
                keys.push_back(elem.first);
                this->save(elem.second);
            }

            m_wiscMsg->set_word_array(std::to_string(keysKey), keys);
        }

        /*!
         * \brief Adds a \c std::map<std::string, T> to the message where \c T is a serializable type
         */
        template<typename T> inline void save(const std::map<std::string, T> &value) {
            // The first RPC key stores the std::map keys
            // This is required to know the std::map size at deserialization
            const auto keysKey = dispenseKey();

            std::vector<std::string> keys{};
            keys.reserve(value.size());

            for (const auto & elem : value) {
                keys.push_back(elem.first);
                this->save(elem.second);
            }

            m_wiscMsg->set_string_array(std::to_string(keysKey), keys);
        }

        /*!
         * \brief Adds the content of a \c void_holder to the message
         *
         * It should be used when setting the result from a function call.
         */
        template<typename T> inline void save(const compat::void_holder<T> &holder) {
            this->save(holder.get());
        }

        /*!
         * \brief Specialization for the \c void special case
         */
        inline void save(compat::void_holder<void>) {}

        /*!
         * \brief Serializes the arguments from a \c std::tuple
         *
         * \c std::tuple content is add from left to right to the message
         * via a recursive template. It should be to serialize function arguments.
         */
        template<std::size_t I = 0,
                 typename... Args,
                 typename std::enable_if<I < sizeof...(Args), int>::type = 0
                >
        inline void save(const std::tuple<Args...> &args) {
            this->save(std::get<I>(args));
            this->save<I+1>(args);
        }

        /*!
         * \brief Terminal call
         */
        template<std::size_t I = 0,
                 typename... Args,
                 typename std::enable_if<I == sizeof...(Args), int>::type = 0
                >
        inline void save(const std::tuple<Args...> &) {}

    public:

        /*!
         * \brief Constructor
         *
         * Data are serialized into the \c wiscMsg message.
         */
        explicit MessageSerializer(wisc::RPCMsg *wiscMsg) noexcept : m_wiscMsg{wiscMsg} {}

        /*!
         * \brief Allows to serialize data into the message with a natural interface
         */
        template<typename T>
        inline MessageSerializer & operator<<(const T &t) {
            this->save(t);
            return *this;
        }

        /*!
         * \brief Behaves as \c operator<<
         *
         * Is used for providing a unifed interface between \c MessageSerializer and
         * \c MessageDeserializer so custom types serialization can be defined in a single
         * function.
         */
        template<typename T>
        inline MessageSerializer & operator&(const T &t) {
            this->save(t);
            return *this;
        }

    };

    /*!
     * \brief This class deserializes parameters from a \c wisc::RPCMsg
     *
     * While it cannot be made \c const because deserializing requires to keep
     * track of the state, this class guarentees that the original \c wisc::RPCMsg
     * object will remain untouched.
     */
    class MessageDeserializer : public MessageBase {

    protected:

        const wisc::RPCMsg *m_wiscMsg;

        /*!
         * \brief Deserializes custom types if possible or else supresses implicit type conversion
         *
         * Every type not defined hereunder is taken care of by this templated function.
         * The function serves two purposes:
         *
         * 1. It delegates the deserialization to a well-known function.
         * 2. It aims at enforcing maximum type compatibility with the UW RPC API by
         *    remembering the developer that she/he can transmit defined types over the
         *    network.
         */
        template<typename T> inline void load(T &t) {
            serialize(*this, t);
        }

        /*!
         * \brief Retrieves a \c std::uint32_t from the message
         */
        inline void load(uint32_t &value) {
            value = m_wiscMsg->get_word(std::to_string(dispenseKey()));
        }

        /*!
         * \brief Retrieves a \c std::vector<std::uint32_t> from the message
         */
        inline void load(std::vector<std::uint32_t> &value) {
            value = m_wiscMsg->get_word_array(std::to_string(dispenseKey()));
        }

        /*!
         * \brief Retrieves a \c std::string from the message
         */
        inline void load(std::string &value) {
            value = m_wiscMsg->get_string(std::to_string(dispenseKey()));
        }

        /*!
         * \brief Retrieves a \c std::vector<std::string> from the message
         */
        inline void load(std::vector<std::string> &value) {
            value = m_wiscMsg->get_string_array(std::to_string(dispenseKey()));
        }

        /*!
         * \brief Retrieves a \c std::array<T> from the message where \c T is an integral type (except \c bool)
         */
        template<typename T,
                 std::size_t N,
                 typename std::enable_if<std::is_integral<T>::value && !helper::is_bool<T>::value, int>::type = 0
                >
        inline void load(std::array<T, N> &value) {
            m_wiscMsg->get_binarydata(std::to_string(dispenseKey()), value.data(), N*sizeof(T));
        }

        /*!
         * \brief Retrieves a \c std::map<std::uint32_t, T> from the message where \c T is a serializable type
         */
        template<typename T> inline void load(std::map<std::uint32_t, T> &value) {
            const auto keys = m_wiscMsg->get_word_array(std::to_string(dispenseKey()));

            for (const auto & key: keys) {
                T val;
                this->load(val);
                value.emplace(key, std::move(val));
            }
        }

        /*!
         * \brief Retrieves a \c std::map<std::string, T> from the message where \c T is a serializable type
         */
        template<typename T> inline void load(std::map<std::string, T> &value) {
            const auto keys = m_wiscMsg->get_string_array(std::to_string(dispenseKey()));

            for (const auto & key: keys) {
                T val;
                this->load(val);
                value.emplace(key, std::move(val));
            }
        }

        /*!
         * \brief Retrieves a \c T parameter from the message and stores it inside
         * a \c void_holder.
         *
         * It should be used when setting the result from a function.
         */
        template<typename T> inline void load(compat::void_holder<T> &value) {
            this->load(value.get());
        }

        /*!
         * \brief Specialization for the \c void special case
         */
        inline void load(compat::void_holder<void>) {}

        /*!
         * \brief Fills in a \c std::tuple with data from the message
         *
         * \c std::tuple content is filled from left to right from the message
         * via a recursive template. It should be use to deserialize function
         * arguments.
         */
        template<std::size_t I = 0,
                 typename... Args,
                 typename std::enable_if<I < sizeof...(Args), int>::type = 0
                >
        inline void load(std::tuple<Args...> &args) {
            this->load(std::get<I>(args));
            this->load<I+1>(args);
        }

        /*!
         * \brief Terminal call
         */
        template<std::size_t I = 0,
                 typename... Args,
                 typename std::enable_if<I == sizeof...(Args), int>::type = 0
                >
        inline void load(std::tuple<Args...> &) { }

    public:

        /*!
         * \brief Constructor
         *
         * Data are retrieved from the provided \c wiscMsg message.
         */
        explicit MessageDeserializer(const wisc::RPCMsg *wiscMsg) noexcept : m_wiscMsg{wiscMsg} {}

        /*!
         * \brief Allows to deserialiaze data from the message with a natural interface
         */
        template <typename T>
        inline MessageDeserializer & operator>>(T &t) {
            this->load(t);
            return *this;
        }

        /*!
         * \brief Behaves as \c operator<<
         *
         * Is used for providing a unifed interface between \c MessageSerializer and
         * \c MessageDeserializer so custom types serialization can be defined in a single
         * function.
         */
        template <typename T>
        inline MessageDeserializer & operator&(T &t) {
            this->load(t);
            return *this;
        }

    };

    /*!
     * \brief Provides a default (de)serialiazer in case the intrusive method is used
     */
    template<typename Message, typename T>
    inline void serialize(Message &msg, T &t) {
        t.serialize(msg);
    }

    /*!
     * \brief Serializer for \c std::array<T, N> where \c is a serializable type
     *
     * This a simple example of custom type serialization.
     *
     * The library provides two custom type serialization methods:
     *
     * 1. The intrusive method
     * 2. The non-intrusive method
     *
     * Let's take an example :
     *
     * \code{.cpp}
     * struct Point
     * {
     *     std::uint32_t x, y;
     *
     *     // The intrusive version is implemented as a new member function
     *     // which takes a message as parameter (i.e. the serializer or deserializer)
     *     template<class Message> inline void serialize(Message & msg) {
     *         msg & x & y;
     *     }
     * };
     *
     * // The non-intrusive version allows to serialize objects defined in a library
     * // Simply define the serialize function in the xhal::rpc namespace or the namespace
     * // where the type is defined with two parameters (1) A message (i.e. the serializer
     * // or deserializer) and (2) the custom type
     * namespace xhal { namspace rpc {
     *     template<typename Message> inline void serialize(Message &msg, Point &point) {
     *         msq & point.x & point.y;
     *     }
     * } }
     * \endcode
     *
     * \warning In order to work as intended the \c serialize functions \b MUST modify
     * the object only with the \c operator&
     */
    template<typename Message, typename T, std::size_t N>
    inline void serialize(Message &msg, std::array<T, N> &value) {
        // The std::array size is known at compile time (and part of
        // the signature), so we don't need to serialize it
        for (auto & elem: value) {
            msg & elem;
        }
    }

} }

#endif
