#include <regex>
#include <gtest/gtest.h>
#include <algorithm>
#include "../Mustache.h"

class TypeOfSynopsis;

TEST(TypeOfSynopsis, AllConditionalInclusion_AndInsertContext) {
    {
        const std::string names = "{{#hi}}Привет, {{name}}!{{/hi}}\n";
        mstch::Mustache a(names);
        {
            std::map<std::string, std::string> aa = {{"name", "Sergey"}};
            ASSERT_EQ(a.generateString(aa), "");
        }
        {
            std::map<std::string, std::string> aa = {{"name", "Sergey"},
                                                     {"hi",   "notempty"}};
            std::string sdf = a.generateString(aa);
            ASSERT_EQ(a.generateString(aa), "Привет, Sergey!\n");
        }
    }
    {
        const std::string names = "{{#hi}}Привет, {{name}}!{{/hi}}\n"
                                  "{{^are}}Are you okay? How are you doing{{/are}}";
        mstch::Mustache a(names);
        {
            std::map<std::string, std::string> aa = {{"name", "Sergey"}};
            ASSERT_EQ(a.generateString(aa), "Are you okay? How are you doing");
        }
        {
            std::map<std::string, std::string> aa = {{"name", "Sergey"},
                                                     {"hi",   "notempty"}};
            ASSERT_EQ(a.generateString(aa), "Привет, Sergey!\nAre you okay? How are you doing");
        }
    }
    {
        const std::string names = "{{^hi}}Привет, {{name}}{{name}}!{{/hi}}\n"
                                  "{{^are}}Are you okay? How are you doing{{/are}}";
        mstch::Mustache a(names);
        {
            std::map<std::string, std::string> aa = {{"name", "Sergey"}};
            ASSERT_EQ(a.generateString(aa), "Привет, SergeySergey!\nAre you okay? How are you doing");
        }
        {
            std::map<std::string, std::string> aa = {{"name", "Sergey"},
                                                     {"hiw",  "notempty"}};
            ASSERT_EQ(a.generateString(aa), "Привет, SergeySergey!\nAre you okay? How are you doing");
        }
    }

}

TEST(TypeOfSynopsis, Comments) {
    {
        const std::string names = "Привет, {{! игнорируемый текст}}Вася!\n"
                                  "{{! тыв  порядке? текст}}как твои дела?\n"
                                  "все хорошо"
                                  "круто";
        mstch::Mustache a(names);
        {
            std::map<std::string, std::string> aa = {{"name", "Sergey"}};

            ASSERT_EQ(a.generateString(aa), "Привет, Вася!\nкак твои дела?\nвсе хорошокруто");
        }
    }
}

TEST(TypeOfSynopsis, Mixed) {
    {
        const std::string names = "Ghbdtn{{#hi}}Привет, {{name}}!{{/hi}}\n"
                                  "{{#love}}\n"
                                  "I love you!\n"
                                  "{{/love}}\n"
                                  "{{^love}}\n"
                                  "I hate you!\n"
                                  "{{/love}}\n"
                                  "Привет, {{! игнорируемый текст}}Вася!\n"
                                  "{{#names}}Hi\n"
                                  "a\n"
                                  "c\n"
                                  "{{name}}!{{/names}}";
        mstch::Mustache a(names);
        {
            std::map<std::string, std::string> aa = {{"name", "Sergey"},
                                                     {"hi",   "empty"},
                                                     {"love", "sd"}};
            EXPECT_EQ(a.generateString(aa), "GhbdtnПривет, Sergey!\nI love you!\nПривет, Вася!");
        }
        {
            std::map<std::string, std::string> ab = {{"name",   "   Strangers   "},
                                                     {"hi",     "empty"},
                                                     {"loves",  "sd"},
                                                     {"nobody", "cares"}};
            std::string sdf = a.generateString(ab);
            EXPECT_EQ(a.generateString(ab), "GhbdtnПривет,    Strangers   !\nI hate you!\nПривет, Вася!");

        }
    }

}