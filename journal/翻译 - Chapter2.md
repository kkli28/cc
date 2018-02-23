# Chapter2. Grammars as a Generating Device

---

> 2.1 Languages as Infinite Sets

```
In computer science as in everyday parlance, a “grammar” serves to “describe” a “language”. If taken at face value, this correspondence, however, is misleading, since the computer scientist and the naive speaker mean slightly different things by the three terms. To establish our terminology and to demarcate the universe of discourse, we shall examine the above terms, starting with the last one.

就像日常用语一样，在计算机科学中，“文法”时用来“描述”一种“语言”的。然而如果从表面上看是具有误导性的，因为计算机科学家和普通的说话者对这三个术语含义的理解时略有不同的。为了建立我们的术语并划定话语世界，我们将从最后一个开始研究上面的术语。
```

> 2.1.1 Language

```
To the larger part of mankind, language is first and foremost a means of communication, to be used almost unconsciously, certainly so in the heat of a debate. Communication is brought about by sending messages, through air vibrations or through written symbols. Upon a closer look the language message (“utterances”) fall apart into sentences, which are composed of words, which in turn consist of symbol sequences when written. Languages can differ on all three levels of composition. The script can be slightly different, as between English and Irish, or very different, as between English and Chinese. Words tend to differ greatly, and even in closely related languages people call un cheval or ein Pferd, that which is known to others as a horse. Differences in sentence structure are often underestimated; even the closely related Dutch often has an almost Shakespearean word order: “Ik geloof je niet”, “I believe you not”, and more distantly related languages readily come up with constructions like the Hungarian “Pénzem van”, “Money-my is”, where the English say “I have money”.

对于大部分人来说，语言首先是一种交流方式，几乎无意识地使用语言，当然在激烈的辩论中也是如此。交流是通过空气振动或通过文字符号携带信息来实现。仔细观察，语言信息会分解成句子，这些句子由单词组成，而单词在书写时又由符号序列组成。语言在所有三个构成层面上都可以不同。字母可能略有不同，如英语和爱尔兰育，或者非常不同，如英语和汉语。语言往往差别很大，甚至在人们熟知的语言中，人们将马称为“un cheval”或“ein Pferd”。句子结构的差异经常被低估。即使是紧密相关的荷兰人，也经常有一种近乎莎士比亚式地语句：“lk geloof je niet”，“i believe you not”，而更多相关的语言很容易找到像匈牙利语“Penzem van”，“money-my is”的结构，而英语中则是“I hava money”。
```

```
The computer scientist takes a very abstracted view of all this. Yes, a language has sentences, and these sentences possess structure; whether they communicate something or not is not his concern, but information may possibly be derived from their structure and then it is quite all right to call that information the “meaning” of the sentence. And yes, sentences consist of words, which he calls “tokens”, each possibly carrying a piece of information, which is its contribution to the meaning of the whole sentence. But no, words cannot be broken down any further. This does not worry the computer scientist. With his love of telescoping solutions and multi-level techniques, he blithely claims that if words turn out to have structure after all, they are sentences in a different language, of which the letters are the tokens.

计算机科学家对这一切都有非常抽象的看法。是的，一种语言有句子，而这些句子具有结构。不管它们是否传达了某种信息，都不是他所关心的，但是信息可能来自于它们的结构，因此把这些信息称为句子的“意义”时完全正确的。是的，句子由单词组成，被称为“token”，每个单词都可能带有一些信息，这就是它对整个句子意义的贡献。但是，语言不能再被分解了，这并不让计算机科学家担心。凭借他对可伸缩的解决方案和多层次技术的热爱，他愉快地宣称，如果单词最终有了结构，它们就是用不同语言写成的句子，其中字母就是“token”。
```

```
The practitioner of formal linguistics, henceforth called the formal-linguist (to distinguish him from the “formal linguist”, the specification of whom is left to the imagination of the reader) again takes an abstracted view of this. A language is a “set” of sentences, and each sentence is a “sequence” of “symbols”; that is all there is: no meaning, no structure, either a sentence belongs to the language or it does not. The only property of a symbol is that it has an identity; in any language there are a certain number of different symbols, the alphabet, and that number must be finite. Just for convenience we write these symbols as a, b, c, . . . , but ✆, ✈, ❐, . . . would do equally well, as long as there are enough symbols. The word sequence means that the symbols in each sentence are in a fixed order and we should not shuffle them. The word set means an unordered collection with all the duplicates removed. A set can be written down by writing the objects in it, surrounded by curly brackets. All this means that to the formal-linguist the following is a language: a, b, ab, ba, and so is {a, aa, aaa, aaaa, . . . } although the latter has notational problems that will be solved later. In accordance with the correspondence that the computer scientist sees between sentence/word and word/letter, the formal-linguist also calls a sentence a word and he says that “the word ab is in the language {a, b, ab, ba}”.

正式语言学的从业者，从此被称为形式语言学家（将他与“正式语言学家”区分开来，后者的详述留给读者想象），再次从抽象的角度来看待这个问题。语言就是句子的“集合”，每个句子都是符号的序列，这就是全部：没有意义，没有结构，任何一个句子都属于一个语言，或者不属于。符号的唯一属性是它有一个标识，在任何语言中都有一定数量的不同符号，即字母，而且数量是有限的。为了方便起见，我们将这些符号写为a，b，c，。。。，但是✆，✈，❐，。。。只要有足够的符号，也一样。单词序列意味着每个句子中的符号都有固定的顺序，我们不应该将它打乱。单词集合意味着这是一个无序的集合，并且去除了重复项。一个集合可以通过花括号括起来，并写入它的对象来写下一组。所有这一切对形式语言学家来说，这是一种语言：a，b，ab，ba，并且{a,aa,aaa,aaaa,...}也是，尽管后者有将在后面解决的计数问题。根据计算机科学家在句子/单词和单词/字母之间看到的对应关系，形式语言学家也把一个句子称为单词，并且说“单词ab属于语言{a, b, ab, ba}”。
```

```
Now let us consider the implications of these compact but powerful ideas.

现在让我们考虑这些紧凑而强大的想法的含义。
```

```
To the computer scientist, a language is a probably infinitely large set of sentences, each composed of tokens in such a way that it has structure; the tokens and the structure cooperate to describe the semantics of the sentence, its “meaning” if you will. Both the structure and the semantics are new, that is, were not present in the formal model, and it is his responsibility to provide and manipulate them both. To a computer scientist 3+4×5 is a sentence in the language of “arithmetics on single digits” (“single digits” to avoid having an infinite number of symbols); its structure can be shown by inserting parentheses: 
(3+ (4×5)); and its semantics is probably 23.

对于计算机科学家来说，语言可能是一个无限大的句子集合，每个句子都是具有结构的记号组成。
记号以及对应的结构配合来描述句子的语义，这就是句子的意义。结构和语义都是新的，也就是说，在形式模型中不存在，计算机科学家的责任就是提供和操作它们。对于计算机科学家来说，3+4*5是一个“个位数算法”语言的句子（“个位数”以避免有无数个符号），它的结构可以通过插入括号来显示：(3+(4*5))，它的语义可能是23。
```

```
To the linguist, whose view of languages, it has to be conceded, is much more normal than that of either of the above, a language is an infinite set of possibly interrelated sentences. Each sentence consists, in a structured fashion, of words which have a meaning in the real world. Structure and words together give the sentence a meaning, which it communicates. Words, again, possess structure and are composed of letters; the letters cooperate with some of the structure to give a meaning to the word. The heavy emphasis on semantics, the relation with the real world and the integration of the two levels sentence/word and word/letters are the domain of the linguist. “The circle spins furiously” is a sentence, “The circle sleeps red” is nonsense.

对于语言学家来说，（============ 待查 ============），语言是一组可能相互关联的句子。每个句子都是以一种结构化的方式，包含在现实世界中具有意义的单词。结构和单词一起赋予了句子传达的意义。单词拥有结构并由字母组成，这写字母和一些结构配合赋予这个单词意义。强调语义与现实世界的关系，以及句子与单词和单词与字母两个层次的整合是语言学家的领域。“轮子猛烈地旋转”是一个句子，而“圆圈睡红”是没有意义的。
```

```
The formal-linguist holds his views of language because he wants to study the fundamental properties of languages in their naked beauty; the computer scientist holds his because he wants a clear, well-understood and unambiguous means of describing objects in the computer and of communication with the computer, a most exacting communication partner, quite unlike a human; and the linguist holds his view of language because it gives him a formal tight grip on a seemingly chaotic and perhaps infinitely complex object: natural language.

形式语言学家持有他对语言的看法，因为他想要研究语言最基本的属性。计算机科学家坚持他的观点，因为他想要一个清晰的、透彻的、明确地方法来描述计算机中的物体以及计算机间的通信，这是一个与人类不同的通信伙伴。语言学家持有他对语言的看法，因为它给了他一个严格控制的看似混乱但也无限复杂的对象：自然语言。
```

> 2.1.2 Grammars （文法）

```
Everyone who has studied a foreign language knows that a grammar is a book of rules and examples which describes and teaches the language. Good grammars make a careful distinction between the sentence/word level, which they often call syntax or syntaxis and the word/letter level, which they call morphology. Syntax contains rules like “pour que is followed by the subjunctive, but parce que is not”. Morphology contains rules like “the plural of an English noun is formed by appending an -s, except when the word ends in -s, -sh, -o, -ch or -x, in which case -es is appended, or when the word has an irregular plural.”

凡是学过外语的人都知道，语法是用来描述和教授这门语言的规则和例子。良好的语法对常称之为语法或句法的句子/单词级别和称之为形态学的单词/字母级别进行了仔细的区分。语法包含像“pour que is followed by the subjunctive, but parce que is not”。形态学包含的规则类似于“英文名词的复数形式是通过追加-s形成的，除非单词以-s，-sh，-o，-ch或-x结尾，在这种情况下追加-es，或当这个词有一个不规则的复数”。
```

```
We skip the computer scientist’s view of a grammar for the moment and proceed immediately to that of the formal-linguist. His view is at the same time very abstract and quite similar to the layman’s: a grammar is any exact, finite-size, complete description of the language, i.e., of the set of sentences. This is in fact the school grammar, with the fuzziness removed. Although it will be clear that this definition has full generality, it turns out that it is too general, and therefore relatively powerless. It includes descriptions like “the set of sentences that could have been written by Chaucer”; platonically speaking this defines a set, but we have no way of creating this set or testing whether a given sentence belongs to this language. This particular example, with its “could have been” does not worry the formal-linguist, but there are examples closer to his home that do. “The longest block of consecutive sevens in the decimal expansion of π” describes a language that has at most one word in it (and then that word will consist of sevens only), and as a definition it is exact, of finite-size and complete. One bad thing with it, however, is that one cannot find this word: suppose one finds a block of one hundred sevens after billions and billions of digits, there is always a chance that further on there is an even longer block. And another bad thing is that one cannot even know if this longest block exists at all. It is quite possible that, as one proceeds further and further up the decimal expansion of π, one would find longer and longer stretches of sevens, probably separated by ever-increasing gaps. A comprehensive theory of the decimal expansion of π might answer these questions, but no such theory exists.

我们暂时跳过计算机科学家关于语法的观点，并立即转向正式语言学家的观点。他的观点非常抽象，且与外行人的观点类似：语法时任何精确的、有限的、完整的对语言（即句子集合）的描述。这实际上是学院风格的语法，带有模糊性。虽然这个定义具有充分的普遍性，但事实证明它太笼统，因此相对无力。它包括诸如“可能由Chaucer创作的句子集合”的描述。理想地说，着定义了一个集合，但是我们没办法创建这个集合或者测试一个给定的句子是否属于这种语言。这个特殊的例子，“本来可以”并不让形式语言学家担心，但有更加接近他的研究领域的例子。“π的十进制展开式中最长连续出现数字7的块”描述了一种语言，其中最多有一个单词（该单词将仅包含数字7），并且它的定义时准确的，即有限且完整的。然而这有一个不好的问题，人们无法找到这个词：假设某人在十亿数字之后找到了一百个由7连续组成的块，那么在这之后还有可能找到一个由更多7组成的块。另一个问题是，人们甚至不知道这个最长的块是否存在。很可能，当π的十进制展开位数越来越多，人们可能会发现更长的由7组成的块，由不断增长的间距间隔。π的小数展开的理论可能可以回答这样的问题，但是没有这样的理论存在。
```

```
For these and other reasons, the formal-linguists have abandoned their static, platonic view of a grammar for a more constructive one, that of the generative grammar: a generative grammar is an exact, fixed-size recipe for constructing the sentences in the language. This means that, following the recipe, it must be possible to construct each sentence of the language (in a finite number of actions) and no others. This does not mean that, given a sentence, the recipe tells us how to construct that particular sentence, only that it is possible to do so. Such recipes can have several forms, of which some are more convenient than others.

由于这些原因，形式语言学家放弃了对语法的静态的、理想的观点，以获得更具建设性的语法观点，即生成语法：生成语法是一种以精确地、固定大小的方法来构造语言中的句子。这意味着，按照既定规则，必须可以无歧义地构建语言中的每个句子（以有限数量的动作）。这并不意味着，给定一个句子，规则告诉我们该如何构造这个特定的句子，它只是可能这样做。规则的形式有多种，其中一些比另一些更方便。
```

```
The computer scientist essentially subscribes to the same view, often with the additional requirement that the recipe should imply how a sentence can be constructed.

计算机科学家基本持有相同的观点，通常还要求规则可以暗示如何构建一个句子。
```

> 2.1.3 Problems with Infinite Sets

```
The above definition of a language as a possibly infinite set of sequences of symbols and of a grammar as a finite recipe to generate these sentences immediately gives rise to two embarrassing questions:
    1. How can finite recipes generate enough infinite sets of sentences?
    2. If a sentence is just a sequence and has no structure and if the meaning of a
sentence derives, among other things, from its structure, how can we assess the meaning of a sentence? These questions have long and complicated answers, but they do have answers. We shall first pay some attention to the first question and then devote the main body of this book to the second.


```
