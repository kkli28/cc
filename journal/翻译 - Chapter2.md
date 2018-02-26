# Chapter2. Grammars as a Generating Device （文法作为生成工具）

---

> 2.1 Languages as Infinite Sets

```
In computer science as in everyday parlance, a “grammar” serves to “describe” a “language”. If taken at face value, this correspondence, however, is misleading, since the computer scientist and the naive speaker mean slightly different things by the three terms. To establish our terminology and to demarcate the universe of discourse, we shall examine the above terms, starting with the last one.

就像日常用语一样，在计算机科学中，“文法”是用来“描述”一种“语言”的。然而如果从表面上看是具有误导性的，因为计算机科学家和普通的说话者对这三个术语含义的理解略有不同。为了建立我们的术语并划定话语世界，我们将从最后一个开始研究上面的术语。
```

> 2.1.1 Language

```
To the larger part of mankind, language is first and foremost a means of communication, to be used almost unconsciously, certainly so in the heat of a debate. Communication is brought about by sending messages, through air vibrations or through written symbols. Upon a closer look the language message (“utterances”) fall apart into sentences, which are composed of words, which in turn consist of symbol sequences when written. Languages can differ on all three levels of composition. The script can be slightly different, as between English and Irish, or very different, as between English and Chinese. Words tend to differ greatly, and even in closely related languages people call un cheval or ein Pferd, that which is known to others as a horse. Differences in sentence structure are often underestimated; even the closely related Dutch often has an almost Shakespearean word order: “Ik geloof je niet”, “I believe you not”, and more distantly related languages readily come up with constructions like the Hungarian “Pénzem van”, “Money-my is”, where the English say “I have money”.

对于大部分人来说，语言首先是一种交流方式，几乎无意识地使用语言，当然在激烈的辩论中也是如此。交流是通过空气振动或通过文字符号携带信息来实现。仔细观察，语言信息会分解成句子，这些句子由单词组成，而单词在书写时又由符号序列组成。语言在所有三个构成层面上都可以不同。字母可能略有不同，如英语和爱尔兰语，或者非常不同，如英语和汉语。语言往往差别很大，甚至在人们熟知的语言中，人们将马称为“un cheval”或“ein Pferd”。句子结构的差异经常被低估。即使是紧密相关的荷兰人，也经常有一种近乎莎士比亚式地语句：“lk geloof je niet”，“i believe you not”，而更多相关的语言很容易找到像匈牙利语“Penzem van”，“money-my is”的结构，而英语中则是“I have money”。
```

```
The computer scientist takes a very abstracted view of all this. Yes, a language has sentences, and these sentences possess structure; whether they communicate something or not is not his concern, but information may possibly be derived from their structure and then it is quite all right to call that information the “meaning” of the sentence. And yes, sentences consist of words, which he calls “tokens”, each possibly carrying a piece of information, which is its contribution to the meaning of the whole sentence. But no, words cannot be broken down any further. This does not worry the computer scientist. With his love of telescoping solutions and multi-level techniques, he blithely claims that if words turn out to have structure after all, they are sentences in a different language, of which the letters are the tokens.

计算机科学家对这一切都有非常抽象的看法。是的，一种语言有句子，而这些句子具有结构。不管它们是否传达了某种信息，都不是他所关心的，但是信息可能来自于它们的结构，因此把这些信息称为句子的“意义”是完全正确的。句子由单词组成，被称为token，每个单词都可能带有一些信息，这就是它对整个句子意义的贡献。但是，语言不能再被分解了，这并不使计算机科学家担心。凭借他对可伸缩的解决方案和多层次技术的热爱，他宣称，如果单词最终有了结构，它们就是用不同语言写成的句子，其中字母就是token。
```

```
The practitioner of formal linguistics, henceforth called the formal-linguist (to distinguish him from the “formal linguist”, the specification of whom is left to the imagination of the reader) again takes an abstracted view of this. A language is a “set” of sentences, and each sentence is a “sequence” of “symbols”; that is all there is: no meaning, no structure, either a sentence belongs to the language or it does not. The only property of a symbol is that it has an identity; in any language there are a certain number of different symbols, the alphabet, and that number must be finite. Just for convenience we write these symbols as a, b, c, . . . , but ✆, ✈, ❐, . . . would do equally well, as long as there are enough symbols. The word sequence means that the symbols in each sentence are in a fixed order and we should not shuffle them. The word set means an unordered collection with all the duplicates removed. A set can be written down by writing the objects in it, surrounded by curly brackets. All this means that to the formal-linguist the following is a language: a, b, ab, ba, and so is {a, aa, aaa, aaaa, . . . } although the latter has notational problems that will be solved later. In accordance with the correspondence that the computer scientist sees between sentence/word and word/letter, the formal-linguist also calls a sentence a word and he says that “the word ab is in the language {a, b, ab, ba}”.

形式语言学的从业者，从此被称为形式语言学家（将他与“正式语言学家”区分开来，后者的详述留给读者想象），再次从抽象的角度来看待这个问题。语言就是句子的集合，每个句子都是符号的序列，没有意义，没有结构，任何一个句子都属于一个语言，或者不属于。符号的唯一属性是它有一个标识，在任何语言中都有一定数量的不同符号，即字母表，而且数量是有限的。为了方便起见，我们将这些符号写为a，b，c，。。。，但是✆，✈，❐，。。。只要有足够的符号就行。单词序列意味着每个句子中的符号都有固定的顺序，我们不应该将它打乱。单词集合意味着这是一个无序的集合，并且去除了重复项。一个集合可以通过花括号括起来，并写入它的对象来写下一组。所对形式语言学家来说，“a, b, ab, ba”是一种语言，并且{a,aa,aaa,aaaa,...}也是，尽管后者有将在后面解决的计数问题。根据计算机科学家在句子/单词和单词/字母之间看到的对应关系，形式语言学家也把一个句子称为单词，并且说“单词ab属于语言{a, b, ab, ba}”。
```

```
Now let us consider the implications of these compact but powerful ideas.

现在让我们考虑这些紧凑而强大的想法的含义。
```

```
To the computer scientist, a language is a probably infinitely large set of sentences, each composed of tokens in such a way that it has structure; the tokens and the structure cooperate to describe the semantics of the sentence, its “meaning” if you will. Both the structure and the semantics are new, that is, were not present in the formal model, and it is his responsibility to provide and manipulate them both. To a computer scientist 3+4×5 is a sentence in the language of “arithmetics on single digits” (“single digits” to avoid having an infinite number of symbols); its structure can be shown by inserting parentheses: 
(3+ (4×5)); and its semantics is probably 23.

对于计算机科学家来说，语言可能是一个无限大的句子集合，每个句子都是具有结构的记号组成。
记号以及对应的结构配合来描述句子的语义，这就是句子的意义。结构和语义都是新的，也就是说，在形式模型中不存在，计算机科学家的责任就是提供和操作它们。对于计算机科学家来说，3+4*5是一个“个位数算法”语言的句子（“个位数”以避免有无数个符号），它的结构可以通过插入括号来显示：(3+(4*5))，它的语义是23。
```

```
To the linguist, whose view of languages, it has to be conceded, is much more normal than that of either of the above, a language is an infinite set of possibly interrelated sentences. Each sentence consists, in a structured fashion, of words which have a meaning in the real world. Structure and words together give the sentence a meaning, which it communicates. Words, again, possess structure and are composed of letters; the letters cooperate with some of the structure to give a meaning to the word. The heavy emphasis on semantics, the relation with the real world and the integration of the two levels sentence/word and word/letters are the domain of the linguist. “The circle spins furiously” is a sentence, “The circle sleeps red” is nonsense.

对于语言学家来说，语言是一组可能相互关联的句子。每个句子都是以一种结构化的方式，包含在现实世界中具有意义的单词。结构和单词一起赋予了句子传达的意义。单词拥有结构并由字母组成，这写字母和一些结构配合赋予这个单词意义。强调语义与现实世界的关系，以及句子与单词和单词与字母两个层次的整合是语言学家的领域。“轮子猛烈地旋转”是一个句子，而“圆圈睡红”是没有意义的。
```

```
The formal-linguist holds his views of language because he wants to study the fundamental properties of languages in their naked beauty; the computer scientist holds his because he wants a clear, well-understood and unambiguous means of describing objects in the computer and of communication with the computer, a most exacting communication partner, quite unlike a human; and the linguist holds his view of language because it gives him a formal tight grip on a seemingly chaotic and perhaps infinitely complex object: natural language.

形式语言学家持有他对语言的看法，因为他想要研究语言最基本的属性。计算机科学家坚持他的观点，因为他想要一个清晰的、透彻的、明确的方法来描述计算机中的物体以及计算机间的通信，这是一个与人类不同的通信伙伴。语言学家持有他对语言的看法，因为它给了他一个严格控制的看似混乱但也无限复杂的对象：自然语言。
```

> 2.1.2 Grammars （文法）

```
Everyone who has studied a foreign language knows that a grammar is a book of rules and examples which describes and teaches the language. Good grammars make a careful distinction between the sentence/word level, which they often call syntax or syntaxis and the word/letter level, which they call morphology. Syntax contains rules like “pour que is followed by the subjunctive, but parce que is not”. Morphology contains rules like “the plural of an English noun is formed by appending an -s, except when the word ends in -s, -sh, -o, -ch or -x, in which case -es is appended, or when the word has an irregular plural.”

凡是学过外语的人都知道，语法是用来描述和教授这门语言的规则和例子。良好的语法对常称之为语法或句法的句子/单词级别和称之为形态学的单词/字母级别进行了仔细的区分。语法包含像“pour que is followed by the subjunctive, but parce que is not”。形态学包含的规则类似于“英文名词的复数形式是通过追加-s形成的，除非单词以-s，-sh，-o，-ch或-x结尾，在这种情况下追加-es，或当这个词有一个不规则的复数”。
```

```
We skip the computer scientist’s view of a grammar for the moment and proceed immediately to that of the formal-linguist. His view is at the same time very abstract and quite similar to the layman’s: a grammar is any exact, finite-size, complete description of the language, i.e., of the set of sentences. This is in fact the school grammar, with the fuzziness removed. Although it will be clear that this definition has full generality, it turns out that it is too general, and therefore relatively powerless. It includes descriptions like “the set of sentences that could have been written by Chaucer”; platonically speaking this defines a set, but we have no way of creating this set or testing whether a given sentence belongs to this language. This particular example, with its “could have been” does not worry the formal-linguist, but there are examples closer to his home that do. “The longest block of consecutive sevens in the decimal expansion of π” describes a language that has at most one word in it (and then that word will consist of sevens only), and as a definition it is exact, of finite-size and complete. One bad thing with it, however, is that one cannot find this word: suppose one finds a block of one hundred sevens after billions and billions of digits, there is always a chance that further on there is an even longer block. And another bad thing is that one cannot even know if this longest block exists at all. It is quite possible that, as one proceeds further and further up the decimal expansion of π, one would find longer and longer stretches of sevens, probably separated by ever-increasing gaps. A comprehensive theory of the decimal expansion of π might answer these questions, but no such theory exists.

我们暂时跳过计算机科学家关于语法的观点，并立即转向正式语言学家的观点。他的观点非常抽象，且与外行人的观点类似：语法是任何精确的、有限的、完整的对语言（即句子集合）的描述。这实际上是学院风格的语法，带有模糊性。虽然这个定义具有充分的普遍性，但事实证明它太笼统，因此相对无力。它包括诸如“可能由Chaucer创作的句子集合”的描述。理想地说，这定义了一个集合，但是我们没办法创建这个集合或者测试一个给定的句子是否属于这种语言。这个特殊的例子，“本来可以”并不让形式语言学家担心，但有更加接近他的研究领域的例子。“π的十进制展开式中最长连续出现数字7的块”描述了一种语言，其中最多有一个单词（该单词将仅包含数字7），并且它的定义是准确的，即有限且完整的。然而这有一个不好的问题，人们无法找到这个词：假设某人在十亿数字之后找到了一百个由7连续组成的块，那么在这之后还有可能找到一个由更多7组成的块。另一个问题是，人们甚至不知道这个最长的块是否存在。很可能，当π的十进制展开位数越来越多，人们可能会发现更长的由7组成的块，由不断增长的间距间隔。π的小数展开的理论可能可以回答这样的问题，但是没有这样的理论存在。
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
    2. If a sentence is just a sequence and has no structure and if the meaning of a sentence derives, among other things, from its structure, how can we assess the meaning of a sentence?
These questions have long and complicated answers, but they do have answers. We shall first pay some attention to the first question and then devote the main body of this book to the second.

上述语言的定义可以产生无限的符号序列，而语法则是产生这些句子的有限方法，这就产生了两个尴尬的问题：
1. 有限的规则如何产生无限的句子？
2. 如果一个句子只是一个序列而没有结构，并且一个句子的含义是从其句子中衍生的，那么我们该如何获知这个句子的意思？
这些问题有很长且很复杂的答案。我们首先关注第一个问题，然后把这本书的主体放在第二个问题上。
```

> 2.1.3.1 Infinite Sets from Finite Descriptions (用有限描述无限)

```
In fact there is nothing wrong with getting a single infinite set from a single finite description: “the set of all positive integers” is a very finite-size description of a definitely infinite-size set. Still, there is something disquieting about the idea, so we shall rephrase our question: “Can all languages be described by finite descriptions?” As the lead-up already suggests, the answer is “No”, but the proof is far from trivial. It is, however, very interesting and famous, and it would be a shame not to present at least an outline of it here.

事实上，从单个有限描述中获得无限集合没有问题：“所有正整数的集合”是一个非常有限大小的无限大集合描述。然而，这个想法有些令人不安，因此我们将重新修改我们的问题：“是否所有的语言都可以用有限的描述来描述？”，正如上面所说的，答案是否定的，但是证明并非无意义。它是非常有趣并且非常有名，有必要在这里提出一个大纲。
```

> 2.1.3.2 Descriptions can be Enumerated

```
The proof is based on two observations and a trick. The first observation is that descriptions can be listed and given a number. This is done as follows. First, take all descriptions of size one, that is, those of only one letter long, and sort them alphabetically. This is the beginning of our list. Depending on what, exactly, we accept as a description, there may be zero descriptions of size one, or 27 (all letters + space), or 95 (all printable ASCII characters) or something similar; this is immaterial to the discussion which follows.

证明基于两个观察和一个技巧。第一个观察是可以列出描述并给出一个数字，如下所示。首先是长度为1的所有描述，即只有一个字母长，并按照字母顺序对它们进行排序。这是我们列表的开始。确切地说，根据我们接受的描述，对于第一个描述，可能有0个，或27个（所有字母和空格），或95个（所有可打印ASCII字符）或类似的东西。这与下面的讨论无关。
```

```
Second, we take all descriptions of size two, sort them alphabetically to give the second chunk on the list, and so on for lengths 3, 4 and further. This assigns a position on the list to each and every description. Our description “the set of all positive integers”, for example, is of size 32, not counting the quotation marks. To find its position on the list, we have to calculate how many descriptions there are with less than 32 characters, say L. We then have to generate all descriptions of size 32, sort them and determine the position of our description in it, say P, and add the two numbers L and P. This will, of course, give a huge number1 but it does ensure that the description is on the list, in a well-defined position; see Figure 2.1.

然后，我们对大小为2的所有描述按字母顺序排列，以给出列表中的第二个块。以此类推长度为3或4，以及更长的单词。这为列表中的每个描述分配了一个位置。例如描述“the set of all positive integers”，它的大小是32，不包括引号。为了找到他们的位置，我们需要计算有多少描述少于32个字符，命名为L。然后我们必须生成关于长度为32的描述，并对其排序，命名为P，并添加L和P。这将给出一个巨大的数字，但它确保描述都在列表的一个明确的位置上。（插入图2.1）
```

```
Two things should be pointed out here. The first is that just listing all descriptions alphabetically, without reference to their lengths, would not do: there are already infinitely many descriptions starting with an “a” and no description starting with a higher letter could get a number on the list. The second is that there is no need to actually do all this. It is just a thought experiment that allows us to examine and draw conclusions about the behavior of a system in a situation which we cannot possibly examine physically.

这里应该指出两点。首先是只按照字母顺序列出所有描述，而不考虑它们的长度，通常不这样做：已经有无限多的以“a”开始的描述，并且没有以较高字母开头的描述可以在列表上得到一个数字。第二个是没有必要这么做。这只是一个思想试验，它让我们在不能进行实地考察时，能够对一个系统的行为进行研究。
```

```
Also, there will be many nonsensical descriptions on the list; it will turn out that this is immaterial to the argument. The important thing is that all meaningful descriptions are on the list, and the above argument ensures that.

此外，列表上还有许多无意义的描述。事实证明，这对论据并不重要。重要的是，所有有意义的描述都在列表中，且上述论点确保了这一点。
```

> 2.1.3.3 Languages are Infinite Bit-Strings (语言时无限长的字符串)

```
We know that words (sentences) in a language are composed of a finite set of symbols; this set is called quite reasonably the “alphabet”. We will assume that the symbols in the alphabet are ordered. Then the words in the language can be ordered too. We shall indicate the alphabet by Σ.

我们知道语言中的单词（句子）是由一组有限的符号组成的，这个集合被称为字母表。我们假设字母表中的符号是有序的。然后语言中的单词也可以排序，我们用Σ来表示字母表。
```

```
Now the simplest language that uses alphabet Σ is that which consists of all words that can be made by combining letters from the alphabet. For the alphabet Σ ={a, b} we get the language { , a, b, aa, ab, ba, bb, aaa, . . . }. We shall call this language Σ∗, for reasons to be explained later; for the moment it is just a name.

最简单的语言是将字母表中的字母组成的所有单词进行结合。对于字母表 Σ ={a, b}，我们可以得到语言{ , a, b, aa, ab, ba, bb, aaa, . . . }。我们称这个语言为Σ*。这将在后面解释原因，暂时它只是一个名字。
```

```
The set notation Σ∗ above started with “ { , a,”, a remarkable construction; the first word in the language is the empty word, the word consisting of zero as and zero bs. There is no reason to exclude it, but, if written down, it may easily be overlooked, so we shall write it as ε (epsilon), regardless of the alphabet. So, Σ∗ = { ε, a, b, aa, ab, ba, bb, aaa, . . . }. In some natural languages, forms of the present tense of the verb “to be” are empty words, giving rise to sentences of the form “I student”, meaning “I am a student.” Russian and Hebrew are examples of this.

Σ*始于“{, a”，语言中的第一个单词是一个空单词，由0个a和0个b组成。并没有理由去掉它，但是如果写下来，它很容易被忽视，所以我们将它写成ε，而不管字母表（字母表中没有该字符）。所以 Σ∗ = { ε, a, b, aa, ab, ba, bb, aaa, . . . }。在一些自然语言中，动词“to be”的现在时形式是空的词，形成了“I student”，意思是“I am a student”。俄语和希伯来语就是这样的例子。
```

```
Since the symbols in the alphabet Σ are ordered, we can list the words in the language Σ∗, using the same technique as in the previous section: First, all words of size zero, sorted; then all words of size one, sorted; and so on. This is actually the order already used in our set notation for Σ∗.

由于字母表Σ中符号是有序的，因此我们可以使用于前一节中相同的技术列出语言Σ*中的单词：首先排序所有大小为0的单词，然后为所有大小为1的单词排序，等等。这实际上是Σ*中已使用的顺序。
```

```
The language Σ∗ has the interesting property that all languages using alphabet Σ are subsets of it. That means that, given another possibly less trivial language over Σ, called L, we can go through the list of words in Σ∗ and put ticks on all words that are in L. This will cover all words in L, since Σ∗ contains any possible word over Σ.

语言Σ∗具有一个有趣的属性，就是所有使用字母表Σ的语言都是它的子集。这意味着，给定另一个比Σ更小的语言，称为L，我们可以遍历Σ∗中的单词列表，并将所有在L中出现的单词取出来，这将覆盖L中的所有单词，因为Σ∗包含Σ中的任何可能单词。
```

```
Suppose our language L is “the set of all words that contain more as than bs”. L is the set {a, aa, aab, aba, baa, . . . }. The beginning of our list, with ticks, will look as follows:
   ε
✔ a
   b
✔ aa
   ab
   ba
   bb
✔ aaa
✔ aab
✔ aba
   abb
✔ baa
   bab
   bba
   bbb
✔ aaaa
... ...

假设我们的语言L是“包含b以外的所有单词的集合”。L是集合{a, aa, aab, aba, baa, . . . }。在列表开头加上记号，看起来如下图所示：（✔ aaaa...图片）
```

```
Given the alphabet with its ordering, the list of blanks and ticks alone is entirely sufficient to identify and describe the language. For convenience we write the blank as a 0 and the tick as a 1 as if they were bits in a computer, and we can now write L = 0101000111010001··· (and Σ∗ = 1111111111111111···). It should be noted that this is true for any language, be it a formal language like L, a programming language like Java or a natural language like English. In English, the 1s in the bitstring will be very scarce, since hardly any arbitrary sequence of words is a good English sentence (and hardly any arbitrary sequence of letters is a good English word, depending on whether we address the sentence/word level or the word/letter level).

根据字母表的顺序，空白和记号列表就足够识别和描述该语言。为了方便起见，我们将空白记为0，将空格记为1，就好像它们是计算机中的位一样，我们可以写出L = 0101000111010001···（Σ∗ = 1111111111111111···）。应该注意的是，这适用于任何语言，像L这样的正式语言，像Java这样的编程语言，或者像英语这样的自然语言。在英语中，位串中的连续的1是非常少的，因为几乎没有任何的任意排序的单词是一个好的英语句子（并且根据我们是在句子/单词或单词/字母水平，几乎没有任何的任意排列的字母序列是一个好的英语单词）。
```

> 2.1.3.4 Diagonalization (对角化)

```
The previous section attaches the infinite bit-string 0101000111010001··· to the description “the set of all the words that contain more as than bs”. In the same vein we can attach such bit-strings to all descriptions. Some descriptions may not yield a language, in which case we can attach an arbitrary infinite bit-string to it. Since all descriptions can be put on a single numbered list, we get, for example, the following picture:
    Description         Language
    Description #1      000000100···
    Description #2      110010001···
    Description #3      011011010···
    Description #4      110011010···
    Description #5      100000011···
    Description #6      111011011···
    ...                 ...

上一节将无限比特串0101000111010001···添加到描述“包含b字母以外的所有单词的集合”，同样的，我们可以将位串添加到所有描述中。某些描述可能不会产生一种语言，我们可以将任意的无限位串添加到其中。由于所有的描述都可以放在有单独编号的列表中，所以我们可以得到下图：
    Description         Language
    Description #1      000000100···
    Description #2      110010001···
    Description #3      011011010···
    Description #4      110011010···
    Description #5      100000011···
    Description #6      111011011···
    ...                 ...
```

```
At the left we have all descriptions, at the right all languages they describe. We now claim that many languages exist that are not on the list of languages above: the above list is far from complete, although the list of descriptions is complete. We shall prove this by using the diagonalization process (“Diagonalverfahren”) of Cantor.

左边所有的描述，定义了右边所有的语言。我们现在断言存在许多不在上述语言列表中的语言：上述列表并不完整，尽管描述列表是完整的。我们将用康托尔的对角论证法来证明这一点。
```

```
Consider the language C = 100110···, which has the property that its n-th bit is unequal to the n-th bit of the language described by Description #n. The first bit of C is a 1, because the first bit for Description #1 is a 0; the second bit of C is a 0, because the second bit for Description #2 is a 1, and so on. C is made by walking the NW to SE diagonal of the language field and copying the opposites of the bits we meet. This is the diagonal in Figure 2.2(a). The language C cannot be on the list! It cannot be on line 1, since its first bit differs (is made to differ, one should say) from that on line 1, and in general it cannot be on line n, since its n-th bit will differ from that on line n, by definition.（图2.2）

考虑语言C=100110...，它具有第n位比特与描述#n的第n位比特不同的属性。C的第一个比特是1，因为描述#1的第1位比特是0。C的第二个比特是0，因为描述#2的第二位比特是1，等等。C是通过沿着西北到东南的对角线，并复制遇到的位的反面来形成的。如图2.2(a)。语言C不可能在列表中！语言C不可能在第一行，因为它的第一个位与第一行的第一个位不同，根据定义，它也不可能在第n行，因为它的第n位为第n行的第n位不同。
```

```
So, in spite of the fact that we have exhaustively listed all possible finite descriptions, we have at least one language that has no description on the list. But there exist more languages that are not on the list. Construct, for example, the language whose n+10-th bit differs from the n+10-th bit in Description #n. Again it cannot be on the list since for every n > 0 it differs from line n in the n+10-th bit. But that means that bits 1. . . 9 play no role, and can be chosen arbitrarily, as shown in Figure 2.2(b); this yields another 2^9 = 512 languages that are not on the list. And we can do even much better than that! Suppose we construct a language whose 2n-th bit differs from the 2n-th bit in Description #n (c). Again it is clear that it cannot be on the list, but now every odd bit is left unspecified and can be chosen freely! This allows us to create freely an infinite number of languages none of which allows a finite description; see the slanting diagonal in Figure 2.2. In short, for every language that can be described there are infinitely many that cannot.

因此，尽管我们已经详尽地列出了所有可能地描述，我们至少有一种语言没有在列表中描述。而且还有更多的语言没有在列表中。例如，构造语言，其n+10位与描述#n的第n+10位不同，它也不可能在列表中，因为对于n>0，其描述的语言的第n+10位都与第n行的第n+10位不同。这意味着第1到9比特不起作用，可以任意选择，如图2.2(b)，这会产生不在列表中的另外2^9=512种语言。我们还可以进一步这样做。假设我们构造了一门语言，它的第2n位与描述#n的第2n位不同，显然它也不可能在列表中，现在每一个奇数位都没有指定，并且可以自由选择。这使我们可以自由创造无限数量的语言，其中没有一种允许有限的描述。参见图2.2中的对角线，简而言之，对于每一种可以描述的语言，都有无限的无法描述的语言。
```

```
The diagonalization technique is described more formally in most books on theoretical computer science; see e.g., Rayward-Smith [393, pp. 5-6], or Sudkamp [397, Section 1.4].

对角化技术在大多数关于计算机科学理论的书籍中描述得更为正式，参见例如Rayward-Smith[393, pp. 5-6]，或Sudkamp[397, Section 1.4]。
```

> 2.1.3.5 Discussion (讨论)

```
The above demonstration shows us several things. First, it shows the power of treating languages as formal objects. Although the above outline clearly needs considerable amplification and substantiation to qualify as a proof (for one thing it still has to be clarified why the above explanation, which defines the language C, is not itself on the list of descriptions; see Problem 2.1, it allows us to obtain insight into properties not otherwise assessable.

上面的演示向我们展示了几件事情。首先，它显示了将语言作为形式对象的能力。虽然上述的内容需要大量的放大和证明才能作为证明的资格（还有一点需要澄清，为什么定义语言C的上述描述本身不在描述列表中，参见问题2.1，它使我们能够获得对不可评估的属性的洞察）。
```

```
Secondly, it shows that we can only describe a tiny subset (not even a fraction) of all possible languages: there is an infinity of languages out there, forever beyond our reach.

其次，它表明了我们只能描述所有可能语言中的一个很小的子集（甚至不是一小部分），这无限的语言，永远超出了我们可触及的范围。
```

```
Thirdly, we have proved that, although there are infinitely many descriptions and infinitely many languages, these infinities are not equal to each other, and the latter is larger than the former. These infinities are called ℵ0 and ℵ1 by Cantor, and the above is just a special case of his proof that ℵ0 < ℵ1.

第三，我们已经证明了，虽然有无穷多的描述和无穷多的语言，但是这些无穷大并不相等，后者比前者更大。Cantor称这些无穷大为ℵ0和ℵ1，以上只是他证明ℵ0<ℵ1的一个特例。
```

> 2.1.4 Describing a Language through a Finite Recipe

```
A good way to build a set of objects is to start with a small object and to give rules for how to add to it and construct new objects from it. “Two is an even number and the sum of two even numbers is again an even number” effectively generates the set of all even numbers. Formalists will add “and no other numbers are even”, but we will take that as understood.

构造一组对象的好方法是从一个小对象开始。“2是偶数，两个偶数的和还是偶数”有效地生成了所有偶数的集合。形式主义者会补充“并且没有其他数字是偶数”，但我们知道这一点。
```

```
Suppose we want to generate the set of all enumerations of names, of the type “Tom, Dick and Harry”, in which all names but the last two are separated by commas. We will not accept “Tom, Dick, Harry” nor “Tom and Dick and Harry”, but we shall not object to duplicates: “Grubb, Grubb and Burrowes”2 is all right. Although these are not complete sentences in normal English, we shall still call them “sentences” since that is what they are in our midget language of name enumerations. A simpleminded recipe would be:
0. Tom is a name, Dick is a name, Harry is a name;
1. a name is a sentence;
2. a sentence followed by a comma and a name is again a sentence;
3. before finishing, if the sentence ends in “, name”, replace it by “and name”.

假设我们想要生成所有类型名为“Tom, Dick and Harry”的枚举集合，其中除了最后两个，所有名称都以逗号分隔。我们不接受“Tom, Dick Harry”或“Tom and Dick and Harry”，但是我们不反对重复：“Grubb, Grubb and Burrowes”是可以的。虽然这些句子都不是正常英语的完整句子，我们依旧叫它们句子，因为这是我们用名词枚举的小型语言。一个简单的规则是：
    0. Tom是一个名字，Dick是一个名字，Harry是一个名字
    1. 一个名字是一个句子
    2. 句子后面跟着逗号，并且名字也是一个句子
    3. 在完成之前，如果句子以“逗号 名字”结尾，则用“and 名字”替换
```

```
Although this will work for a cooperative reader, there are several things wrong with it. Clause 3 is especially wrought with trouble. For example, the sentence does not really end in “, name”, it ends in “, Dick” or such, and “name” is just a symbol that stands for a real name; such symbols cannot occur in a real sentence and must in the end be replaced by a real name as given in clause 0. Likewise, the word “sentence” in the recipe is a symbol that stands for all the actual sentences. So there are two kinds of symbols involved here: real symbols, which occur in finished sentences, like “Tom”, “Dick”, a comma and the word “and”; and there are intermediate symbols, like “sentence” and “name” that cannot occur in a finished sentence. The first kind corresponds to the words or tokens explained above; the technical term for them is terminal symbols(or terminalsfor short). The intermediate symbols are called nonterminals, a singularly uninspired term. To distinguish them, we write terminals in lower case letters and start non-terminals with an upper case letter. Non-terminals are called (grammar) variables or syntactic categories in linguistic contexts.

尽管这对一起合作的读者来说是有效的，但是依旧存在几个问题。特别是第三条。例如句子没有以“逗号 名字”结尾，而是以“逗号 Dick”结尾，名字只是代表真实名称的符号，这些符号不能出现在真正的句子中，最后必须用第0条中给出的真实名称代替。同样，规则中的“句子”一词代表所有实际句子的符号。这里涉及两种符号：出现在完整句子中的真正的符号，如“Tom”，“Dick”，逗号和“and”，还有一些中间符号，如“句子”和“名字”，他们不能再一个完整句子中出现。第一种对应于上面解释的单词或标记，它们的专业术语时终结符（或简称终结）。中间的符号被称为非终结符，一个没有启发性的术语。为了区分它们，用小写字母书写终结符，而用大写字母书写非终结符。非终结符在语言上下文中称为变量或句法类别。
```

```
To stress the generative character of the recipe, we shall replace “X is a Y” by “Y may be replaced by X”: if “tom” is an instance of a Name, then everywhere we have a Name we may narrow it down to “tom”. This gives us: 
    0. Name may be replaced by “tom”
        Name may be replaced by “dick”
        Name may be replaced by “harry”
    1. Sentence may be replaced by Name
    2. Sentence may be replaced by Sentence, Name
    3. “, Name” at the end of a Sentence must be replaced by “and Name” before Name is replaced by any of its replacements
    4. a sentence is finished only when it no longer contains non-terminals
    5. we start our replacement procedure with Sentence

Clause 0 through 3 describe replacements, but 4 and 5 are different. Clause 4 is not specific to this grammar. It is valid generally and is one of the rules of the game. Clause 5 tells us where to start generating. This name is quite naturally called the start symbol, and it is required for every grammar.

为了强调规则的生成特性，我们将用“Y可能被X替换”来代替“X是Y”：假如“tom”时一个名字的实例，则无论任何地方有一个名字，我们都可以用“tom”代替。这告诉我们：
    0. 名称可以用“tom”、“dick”、“harry”代替
    1. 句子可以用名字代替
    2. 句子可以用句子、名字代替
    3. 名字被替换前，出现在句子末尾的“, 名字”必须用“and 名字”代替
    4. 只有当句子不再包含非终结符时，句子才结束
    5. 我们用句子开始替换程序
第0-3条描述替换规则，而第4条和第5条不同。第四条不是针对文法的，它通常时有效的且是必要规则之一。第五条告诉我们从哪里开始生成。这个名称很自然地被称为初始符，它是每个文法所必须的。
```

```
Clause 3 still looks worrisome; most rules have “may be replaced”, but this one has “must be replaced”, and it refers to the “end of a Sentence”. The rest of the rules work through replacement, but the problem remains how we can use replacement to test for the end of a Sentence. This can be solved by adding an end marker after it. And if we make the end marker a non-terminal which cannot be used anywhere except in the required replacement from “, Name” to “and Name”, we automatically enforce the restriction that no sentence is finished unless the replacement test has taken place. For brevity we write ---> instead of “may be replaced by”; since terminal and non-terminal symbols are now identified as technical objects we shall write them in a typewriter-like typeface. The part before the ---> is called the left-hand side, the part after it the right-hand side. This results in the recipe in Figure 2.3.

第3条依旧看起来不是那么清晰，大多数规则都有“可能被替换”，但是这个规则有“必须被替换”，且它指的是“句子的结尾”。其余的规则通过替换来完成，但问题是我们如何测试句子的结尾。这可以通过在它之后添加一个结束标记来解决。如果我们将结束标记设置为非终结符，除了从“, 名称”到“and 名称”的替换外任何地方都无法使用，我们会自动执行除非进行替换测试，否则不会完成句子的限制。为了简洁起见，我们写 ---> 而不是“可以替换”，因为终结符和非终结符现在被确定为技术对象，所以我们将用类似打字机的字体来书写他们。在--->之前的部分称为左侧，之后的部分称为右侧，规则的结果如图2.3。（图2.3）

0.      Name ---> tom
        Name ---> dick
        Name ---> harry
1.  Sentence ---> Name
    Sentence ---> List End
2.      List ---> Name
        List ---> List , Name
3., Name End ---> and Name
4. the start symbol is Sentence

Fig. 2.3. A finite recipe for generating strings in the t, d & h language
```

```
This is a simple and relatively precise form for a recipe, and the rules are equally straightforward: start with the start symbol, and keep replacing until there are no non-terminals left.

这是一个简单且相对精确的形式，规则同样简单：从开始符开始，一直替换直到右侧没有非终结符。
```

> 2.2 Formal Grammars (形式文法)

```
The above recipe form, based on replacement according to rules, is strong enough to serve as a basis for formal grammars. Similar forms, often called “rewriting systems”, have a long history among mathematicians, and were already in use several centuries B.C. in India (see, for example, Bhate and Kak [411]). The specific form of Figure 2.3 was first studied extensively by Chomsky [385]. His analysis has been the foundation for almost all research and progress in formal languages, parsers and a considerable part of compiler construction and linguistics.

上面的规则形式，根据规则进行替换，足以作为正式语法的基础。类似的形式，通常称为“重写系统”，在数学家中有着悠久的历史，且已经在印度使用了几个世纪（参见Bhate和Kak[411]）。图2.3的具体形式首先被Chomskey[385]广泛研究。他的研究是几乎所有形式语言、解析器、一部分编译器构造和语言学的基础。
```

> 2.2.1 The Formalism of Formal Grammars

```
Since formal languages are a branch of mathematics, work in this field is done in a special notation. To show some of its flavor, we shall give the formal definition of a grammar and then explain why it describes a grammar like the one in Figure 2.3. The formalism used is indispensable for correctness proofs, etc., but not for understanding the principles; it is shown here only to give an impression and, perhaps, to bridge a gap.

由于形式语言是数学的一个分支，这个领域的工作是使用一种特殊的符号来完成的。为了展示它的风格，我们将给出一个语法的正式定义，然后解释为什么它描述了一个如图2.3中所示的语法。所使用的形式体系对于正确性证明时必不可少的，但不是用于理解这些原则。在这里展示仅仅是给人一种印象，或填补空白。
```

```
Definition 2.1: A generative grammar is a 4-tuple (VN,VT ,R,S) such that
(1) VN and VT are finite sets of symbols,
(2) VN ∩VT = 0/,
(3) R is a set of pairs (P,Q) such that
(3a) P ∈ (VN ∪VT )+ and
(3b) Q ∈ (VN ∪VT )∗,
(4) S ∈ VN

定义2.1：生成文法时一个四元组(VN, VT, R, S)，如：
....
```

```
A 4-tuple is just an object consisting of 4 identifiable parts; they are the nonterminals, the terminals, the rules and the start symbol, in that order. The above definition does not tell this, so this is for the teacher to explain. The set of nonterminals is named VN and the set of terminals VT . For our grammar we have:
    VN = {Name, Sentence, List, End}
    VT = {tom, dick, harry, ,, and}
    (note the , in the set of terminal symbols).

四元组只是一个由四个可识别部分组成的对象，它们是按照非终结符，终结符，规则和初始符的顺序排列。上面的定义并没有说明这一点，所以这是老师要解释的。非终结符集合称为VN，终结符集合称为VT。对于我们的文法，有：
    VN = {Name, Sentence, List, End}
    VT = {tom, dick, harry, ,, and}
    （注意，逗号“，”在终结符集中）
```

```
The intersection of VN and VT (2) must be empty, indicated by the symbol for the empty set, 0/. So the non-terminals and the terminals may not have a symbol in common, which is understandable.

VN和VT的交集必须为空，由空集符号（======== 空集符号 ========）表示。因此非终结符和终结符没有一个共同符号，这是可以理解的。
```

```
R is the set of all rules (3), and P and Q are the left-hand sides and right-hand sides, respectively. Each P must consist of sequences of one or more non-terminals and terminals and each Q must consist of sequences of zero or more non-terminals and terminals. For our grammar we have:
    R = {(Name, tom), (Name, dick), (Name, harry),
    (Sentence, Name), (Sentence, List End), (List, Name),
    (List, List , Name), (, Name End, and Name)}
Note again the two different commas.

R是所有规则的集合，P和Q分别是左侧和右侧。每个P必须由一个或多个非终结符序列组成，每个Q必须由零个或多个非终结符或终结符序列组成。对于我们的文法，有：
    R = {(Name, tom), (Name, dick), (Name, harry),
    (Sentence, Name), (Sentence, List End), (List, Name),
    (List, List , Name), (, Name End, and Name)}
注意两个不同的逗号。
```

```
The start symbol S must be an element of VN, that is, it must be a non-terminal:
    S = Sentence

初始符S必须是VN的一个元素，也就是说它必须是一个非终结符：
    S = Sentence
```

```
This concludes our field trip into formal linguistics. In short, the mathematics of formal languages is a language, a language that has to be learned; it allows very concise expression of what and how but gives very little information on why. Consider this book a translation and an exegesis.

这结束了我们对正式语言学的考察，简而言之，正式语言的数学是一门语言，一门必须学习的语言，它可以非常简洁地表达什么和怎样，但是却很少提供关于原因的信息。考虑这本书的翻译和注释。
```

> 2.2.2 Generating Sentences from a Formal Grammar

```
The grammar in Figure 2.3 is what is known as a phrase structure grammar for our t,d&h language (often abbreviated to PS grammar). There is a more compact notation, in which several right-hand sides for one and the same left-hand side are grouped together and then separated by vertical bars, |. This bar belongs to the formalism, just as the arrow --->, and can be read “or else”. The right-hand sides separated by vertical bars are also called alternatives. In this more concise form our grammar becomes
    0. Name ---> tom | dick | harry
    1. Sentence_s ---> Name | List End
    2. List ---> Name | Name , List
    3. , Name End ---> and Name
where the non-terminal with the subscript s is the start symbol. (The subscript identifies the symbol, not the rule.)

图2.3中的文法时我们语言的短语结构语法（通常缩写为PS语法）。有一种更简洁的语法，其中相同左侧的多个右侧对象被组合在一起，然后用竖线“|”分开。这个分隔符属于形式定义，就像箭头--->一样，可以被读作“或者”。由竖线分隔的右侧对象也被称为替代品。在这个更简洁的形式中，我们的文法变成了：
    0. Name ---> tom | dick | harry
    1. Sentence_s ---> Name | List End
    2. List ---> Name | Name , List
    3. , Name End ---> and Name
带有下标_s的非终结符是开始符号（下标标记的是符号，而不是规则）。
```

```
Now let us generate our initial example from this grammar, using replacement according to the above rules only. We obtain the following successive forms for Sentence:

Intermediate form       Rule used                   Explanation
Sentence                                            the start symbol
List End                Sentence ---> List End      rule 1
Name , List End         List ---> Name , List       rule 2
Name , Name , List End  List ---> Name , List       rule 2
Name , Name , Name End  List ---> Name              rule 2
Name , Name and Name    , Name End ---> and Name    rule 3
tom , dick and harry                                rule 0, three times

The intermediate forms are called sentential forms. If a sentential form contains no non-terminals it is called a sentence and belongs to the generated language. The transitions from one line to the next are called production steps and the rules are called production rules, for obvious reasons.

现在让我们从这个文法中生成初始示例，只根据上述规则使用替换。我们获得下列连续形式的句子：

中间类型                   使用的规则                 说明
Sentence                                            初始符
List End                Sentence ---> List End      规则1
Name , List End         List ---> Name , List       规则2
Name , Name , List End  List ---> Name , List       规则2
Name , Name , Name End  List ---> Name              规则2
Name , Name and Name    , Name End ---> and Name    规则3
tom , dick and harry                                3次规则0

中间形式被称为句型。如果一个句型不包含非终结符则称为句子，并且属于生成的语言。
```

```
The production process can be made more visual by drawing connective lines between corresponding symbols, using a “graph”. A graph is a set of nodes connected by a set of edges. A node can be thought of as a point on paper, and an edge as a line, where each line connects two points; one point may be the end point of more than one line. The nodes in a graph are usually “labeled”, which means that they have been given names, and it is convenient to draw the nodes on paper as bubbles with their names in them, rather than as points. If the edges are arrows, the graph is a directed graph; if they are lines, the graph is undirected. Almost all graphs used in parsing techniques are directed.

通过使用“图形”在相应符号之间绘制连接线，可以使生产过程更加可视化。图是由一组边连接起来的一组节点。一个节点可以被看做是纸上的一个点，边是一条线，每条线连接两个节点。一个点可能不止是一条线的端点。图中的节点通常是“被标记”的，这意味着它们被赋予了名称，而不仅仅是一个点。假如边是箭头，则图是一个有向图，如果边是线条，则图是无向的。几乎所有用于解析技术的图都是有向的。
```

```
The graph corresponding to the above production process is shown in Figure 2.4. Such a picture is called a production graph or syntactic graph and depicts the syntactic structure (with regard to the given grammar) of the final sentence. We see that the production graph normally fans out downwards, but occasionally we may see starlike constructions, which result from rewriting a group of symbols.

与上述生产过程对应的图如图2.4所示。这样的图片称为生产图或句法图，描述了最终句子的句法结构（关于给定的文法）。我们看到生产图通常是扇面向下的，但偶尔也会看到因重写一组符号而导致的星型结构。
```

```
A cycle in a graph is a path from a node N following the arrows, leading back to N. A production graph cannot contain cycles; we can see that as follows. To get a cycle we would need a non-terminal node N in the production graph that has produced children that are directly or indirectly N again. But since the production process always makes new copies for the nodes it produces, it cannot produce an already existing node. So a production graph is always “acyclic”; directed acyclic graphs are called dags.

图中的环是一条从带着箭头的N开始，返回到N的路径。生产图不能包含环。我们将在后面看到，为了得到了一个环，我们需要在生产图中有一个非终结符节点N，该节点直接或间接生成子节点N。但是由于生产过程中总是为其生产的节点生成一个副本，所以它无法生成一个已经存在的节点。因此生产图总是“无环”的，有向无环图称为dag。
```

```
It is patently impossible to have the grammar generate tom, dick, harry, since any attempt to produce more than one name will drag in an End and the only way to get rid of it again (and get rid of it we must, since it is a non-terminal) is to have it absorbed by rule 3, which will produce the and. Amazingly, we have succeeded in implementing the notion “must replace” in a system that only uses “may replace”; looking more closely, we see that we have split “must replace” into “may replace” and “must not be a non-terminal”.

文法产生“tom, dick, harray”是不可能的，因为任何试图产生多个名字的尝试都会带有一个“End”，而唯一可以摆脱它的方法（我们必须摆脱它，因为它是一个非终结符）是被规则3吸收，这将产生“and”。令人惊讶的是，我们成功实现了“必须替换”的概念，而这个系统只用“可能替换”。仔细观察，我们发现我们已经将“必须替换”分为“可能替换”和“不能是非终结符”。
```

```
Apart from our standard example, the grammar will of course also produce many other sentences; examples are
    
    harry and tom
    harry
    tom, tom, tom and tom

and an infinity of others. A determined and foolhardy attempt to generate the incorrect form without the and will lead us to sentential forms like

    tom, dick, harry End

which are not sentences and to which no production rule applies. Such forms are called blind alleys. As the right arrow in a production rule already suggests, the rule may not be applied in the reverse direction.

出了我们的标准例子，文法也可以产生许多其他的句子。例如：
    harry and tom
    harry
    tom, tom, tom and tom
以及无限的其他的句子。如果硬是要产生不包含“and”的错误形式的句子，将会导致如下句型：
    tom, dick harry End
这并不是句子，也不适合任何产生式规则。这种形式被称为盲区。正如产生式规则中的向右箭头一样，规则不能被反向使用。
```

> 2.2.3 The Expressive Power of Formal Grammars （形式文法的表现力）

```
The main property of a formal grammar is that it has production rules, which may be used for rewriting part of the sentential form (= sentence under construction) and a starting symbol which is the mother of all sentential forms. In the production rules we find non-terminals and terminals; finished sentences contain terminals only. That is about it: the rest is up to the creativity of the grammar writer and the sentence producer.

形式文法的主要属性是它有产生规则，可以用来重写部分句子结构的一部分和初始符，它是所有句型的母亲。在产生式中，我们发现了非终结符和终结符，完整的句子只包含终结符。而其余的部分由文法作者和句子的生产者决定。
```

```
This is a framework of impressive frugality and the question immediately rises: Is it sufficient? That is hard to say, but if it is not, we do not have anything more expressive. Strange as it may sound, all other methods known to mankind for generating sets have been proved to be equivalent to or less powerful than a phrase structure grammar. One obvious method for generating a set is, of course, to write a program generating it, but it has been proved that any set that can be generated by a program can be generated by a phrase structure grammar. There are even more arcane methods, but all of them have been proved not to be more expressive. On the other hand there is no proof that no such stronger method can exist. But in view of the fact that many quite different methods all turn out to halt at the same barrier, it is highly unlikely3 that a stronger method will ever be found. See, e.g. Révész [394, pp 100-102].

这是一个令人印象深刻而简洁的框架，这立即产生了一个问题：这足够吗？这很难说，但是如果这都还不够，我们就再没有更具表现力的东西了。尽管听起来很奇怪，但人类已知的所有其他生成集合的方法都被证明与短语结构文法相当或更弱。当然，生成一个集合的一个显而易见的方法是写一个程序来生成它，但是任何可以由程序生成的集合都被证明可以由短语结构文法来生成。甚至还有更加神秘的方法，但是都被证明不更具表现力。另一方面，也没有证据表明不存在这种更强大的方法。但是，鉴于许多不同的方法都遇到各种障碍，很难找到更强大的方法。参见Révész [394, pp 100-102]。
```

```
As a further example of the expressive power we shall give a grammar for the movements of a Manhattan turtle. A Manhattan turtle moves in a plane and can only move north, east, south or west in distances of one block. The grammar of Figure 2.5 produces all paths that return to their own starting point. As to rule 2, it should be
    1.  Moves ---> north Move south | east Move west | ε
    2.  north east ---> east north
        north south ---> south north
        north west ---> west north
        east north ---> north east
        east south ---> south east
        east west ---> west east
        south north ---> north south
        south east ---> east south
        south west ---> west south
        west north ---> north west
        west east ---> east west
        west south ---> south west
    Fig. 2.5. Grammar for the movements of a Manhattan turtle

noted that many authors require at least one of the symbols in the left-hand side to be a non-terminal. This restriction can always be enforced by adding new non-terminals.

作为表现力的另一个例子，我们将给出一个曼哈顿乌龟运动的文法。曼哈顿乌龟在平面上移动，只能往北、东、南或西移动一个方块的位置。图2.5的文法生成了返回到它自己起点的所有路径。关于规则2，应该注意的是，许多作者要求左侧至少有一个非终结符。这个限制总是可以通过添加新的非终结符来实施。
    1.  Moves ---> north Move south | east Move west | ε
    2.  north east ---> east north
        north south ---> south north
        north west ---> west north
        east north ---> north east
        east south ---> south east
        east west ---> west east
        south north ---> north south
        south east ---> east south
        south west ---> west south
        west north ---> north west
        west east ---> east west
        west south ---> south west
    Fig. 2.5. Grammar for the movements of a Manhattan turtle
```

```
The simple round trip north east south west is produced as shown in Figure 2.6 (names abbreviated to their first letter). Note the empty alternative in rule 1 (the ε), which results in the dying out of the third M in the above production graph.

简单往返东北西南方向的产生式如图2.6所示（名称缩写为一个字母）。注意规则1中的空白选择（ε），导致了图中第3个M消失。
```
