#pragma once
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <tuple>
#include <type_traits>
#include <stdexcept>
#include <functional>
#include <any>
#include <utility>
#include <memory>
#include <exception>
#include <iostream>

#define DEBUG_WRAPPER 1

template<class Class, class Function>
class wrapper
{
    static_assert(std::is_class<Class>::value,
                  "Class must be a class type");
    static_assert(std::is_member_function_pointer_v<Function>,
         "Function must be a member function pointer");

private:
    template<typename T>
    struct methodTraits;

    template<typename C, typename R, typename... Args>
    struct methodTraits<R (C::*)(Args...)>
    {
        using returnType = R;
        using argsTuple = std::tuple<Args...>;
        static constexpr size_t argCount = sizeof...(Args);
        using argTypes = std::tuple<Args...>;
        
        template<size_t Index>
        using argType = typename std::tuple_element<Index, std::tuple<Args...>>::type;
    };

    using traits = methodTraits<Function>;

    std::shared_ptr<Class> _instance;
    Function _function;
    std::vector<std::any> defaultArgs;
    std::vector<std::string> argNames;
    std::unordered_map<std::string, size_t> argsIndexMap;

    void makeDefaultTuple(const std::vector<std::pair<std::string,std::any>>& defaults)
    {
        defaultArgs.resize(defaults.size());
        for (size_t i = 0; i < defaults.size(); ++i){
            defaultArgs[i] = defaults[i].second;
        }
    }

    template<std::size_t I>
    auto selectArg(const std::unordered_map<std::string,std::any>& overrides) const
    {
        using ArgType = typename traits::argType<I>;

        const std::string& name = argNames[I];
        auto it = overrides.find(name);
        
        if (it != overrides.end()) {
            return std::any_cast<ArgType>(it->second);
        }

        return std::any_cast<ArgType>(defaultArgs[I]);
    }

    template<std::size_t... I>
    typename traits::argsTuple makeArgsTuple(const std::unordered_map<std::string, std::any>& overrides, std::index_sequence<I...>) const
    {
        return std::make_tuple(selectArg<I>(overrides)...);
    }

    wrapper(std::shared_ptr<Class> instance, Function function, const std::vector<std::pair<std::string,std::any>>& defaults)
        : _instance(instance), _function(function)
    {
        if (defaults.size() != traits::argCount){
            throw std::runtime_error("Default arguments size must match function parameter count");   
        }

        argNames.resize(traits::argCount);
        for (size_t i = 0; i < defaults.size(); ++i)
        {
            argNames[i] = defaults[i].first;
            argsIndexMap[argNames[i]] = i;
        }

        makeDefaultTuple(defaults);
    }

    template<typename C2, typename F2>
    friend auto makeWrapper(std::shared_ptr<C2> instance,
                            F2 function, 
                            const std::vector<std::pair<std::string, std::any>>& defaultArgs);

public:
    typename traits::returnType execute(const std::vector<std::pair<std::string, std::any>>& overridesVec)
    {
        std::unordered_map<std::string, std::any> overrides;
        for (const auto& p : overridesVec) {
            overrides[p.first] = p.second;
        }

        typename traits::argsTuple argsTupleFinal = makeArgsTuple(overrides, std::make_index_sequence<traits::argCount>{});

        return std::apply([this](auto&&... unpacked){
            return std::invoke(_function, _instance.get(), unpacked...);
        }, argsTupleFinal);
    }
    
    using ReturnType = typename traits::returnType;

    static const std::type_info& getReturnTypeInfo() {
        return typeid(typename traits::returnType);
    }
    
};

template<typename C, typename F>
auto makeWrapper(std::shared_ptr<C> instance,
                F function, 
                const std::vector<std::pair<std::string, std::any>>& defaultArgs)
{
    return wrapper<C,F>(instance, function, defaultArgs);
}