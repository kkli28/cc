# Chapter1. Introduction

---

```
Parsing is the process of structuring a linear representation in accordance with a given grammar. This definition has been kept abstract on purpose to allow as wide an program, a knitting pattern, a sequence of geological strata, a piece of music, actions some way restrict the next element. For some of the examples the grammar is well known, for some it is an object of research, and for some our notion of a grammar is only just beginning to take shape.

解析是根据给定的文法构造线性表示的过程。这个定义一直是抽象的，目的是为了让一个程序，一个编织图案，一个地质层次的序列，一段音乐，动作以某种方式限制下一个元素。对于一些例子来说，文法是众所周知的，对一些人来说，它是研究的对象，对于一些人来说，文法的概念才刚刚开始成形。
```

```
For each grammar, there are generally an infinite number of linear representations (“sentences”) that can be structured with it. That is, a finite-size grammar can supply structure to an infinite number of sentences. This is the main strength of the grammar paradigm and indeed the main source of the importance of grammars: they summarize succinctly the structure of an infinite number of objects of a certain class.

对于每个文法，通常有无数个可以用它来构造的线性表示（句子）。也就是说，有限大小的文法可以构造出无限多个句子。这就是文法范式的主要力量，也是文法的重要来源：它们简明扼要地总结了无穷多个某一类对象的结构。
```

```
There are several reasons to perform this structuring process called parsing. One reason derives from the fact that the obtained structure helps us to process the object further. When we know that a certain segment of a sentence is the subject, that information helps in understanding or translating the sentence. Once the structure of a document has been brought to the surface, it can be converted more easily.

执行称为解析的结构化过程有几个原因。其中一个原因是得到的结构有助于我们进一步处理这个物体。当我们知道一个句子的某个部分是主语时，这个信息可以帮助理解和翻译句子。一旦文件的结构被识别，它就可以更容易地被转换。
```

```
A second reason is related to the fact that the grammar in a sense represents our understanding of the observed sentences: the better a grammar we can give for the movements of bees, the deeper our understanding is of them.

第二个原因是文法在某种意义上代表着我们对被观察句子的理解。我们对蜜蜂的运动给出的文法越好，我们对它的理解就越深。
```

```
A third lies in the completion of missing information that parsers, and especially error-repairing parsers, can provide. Given a reasonable grammar of the language, an error-repairing parser can suggest possible word classes for missing or unknown words on clay tablets.

第三个是因为解析器，特别是带有错误恢复功能的解析器能够提供对丢失信息的补全。给予语言的一个有效文法，带有错误恢复功能的解析器能够对丢失或未知的单词给出建议。
```

``` 
The reverse problem — given a (large) set of sentences, find the/a grammar which produces them — is called grammatical inference. Much less is known about it than about parsing, but progress is being made. The subject would require a complete book. Proceedings of the International Colloquiums on Grammatical Inference are published as Lecture Notes in Artificial Intelligence by Springer.

相反的问题：给出一个（大量）句子，找出产生它们的文法，被称为文法推理。人们对它的了解远少于对解析的了解，但是正在取得进展。这个问题需要一本完整的书。关于语法推理的国际研讨会论文集被Springer作为人工智能课堂讲稿发表。
```

> 1.1 Parsing as a Craft (解析作为一个工艺)

```
Parsing is no longer an arcane art; it has not been so since the early 1970s when Aho, Ullman, Knuth and many others put various parsing techniques solidly on their theoretical feet. It need not be a mathematical discipline either; the inner workings of a parser can be visualized, understood and modified to fit the application, with little more than cutting and pasting strings.

自从20世纪70年代初，Aho，Ullman，Knuth和其他许多人把各种各样的解析技术应用到他们的理论中，解析技术就不再是一门神秘的艺术了。它也不是一门数学学科，解析器的内部工作方式可以可视化、理解和修改以适应应用程序，只需剪切和粘贴字符串。
```

```
There is a considerable difference between a mathematician’s view of the world and a computer scientist’s. To a mathematician all structures are static: they have always been and will always be; the only time dependence is that we just have not discovered them all yet. The computer scientist is concerned with (and fascinated by) the continuous creation, combination separation and destruction of structures: time is of the essence. In the hands of a mathematician, the Peano axioms create the integers without reference to time, but if a computer scientist uses them to implement integer addition, he finds they describe a very slow process, which is why he will be looking for a more efficient approach. In this respect the computer scientist has more in common with the physicist and the chemist; like them, he cannot do without a solid basis in several branches of applied mathematics, but, like them, he is willing (and often virtually obliged) to take on faith certain theorems handed to him by the mathematician. Without the rigor of mathematics all science would collapse but not all inhabitants of a building need to know all the spars and girders that keep it upright. Factoring out certain detailed knowledge to specialists reduces the intellectual complexity of a task, which is one of the things computer science is about.

数学家对世界的看法和计算机科学家的观点有很大的不同。对一个数学家而言，所有的结构都是静态的，它们一直都是，将来也会是，唯一跟时间相关的就是我们还没有发现所有的结构。计算机科学家关心（并着迷于）不断的创造、组合分离和结构的破坏：时间才是本质。在数学家手中，Peano公理在不引用时间的情况下创建了整数，但是如果一个计算机科学家使用它们来实现整数假发，他发现他们描述的是一个非常缓慢的过程，这就是为什么他要寻找一个更有效的方法。在这个方面，计算机科学家和物理学家以及化学家有很多共同之处，和他们一样，他在应用数学的几个分支中不能没有坚实的基础，但是也像他们一样，他愿意（通常时有义务）相信数学家交给他的某些定理。没有数学的严谨，所有学科都会崩溃，但是并非所有的居民都需要知道所有能保持直立的桅杆和大梁。将某些详细的知识分解给专家，可以降低任务的智力复杂性，这是计算机科学的重点之一。
```

```
This is the vein in which this book is written: parsing for anybody who has parsing to do: the compiler writer, the linguist, the database interface writer, the geologist or musicologist who wants to test grammatical descriptions of their respective objects of interest, and so on. We require a good ability to visualize, some programming experience and the willingness and patience to follow non-trivial examples; there is nothing better for understanding a kangaroo than seeing it jump. We treat, of course, the popular parsing techniques, but we will not shun some weird techniques that look as if they are of theoretical interest only: they often offer new insights and a reader might find an application for them.

这就是本书所写的脉络：对所有想要测试他们各自感兴趣的对象的文法描述的人进行解析：编译器的作者、语言学家、数据库接口作者、地质学家或音乐学者等。我们需要一个良好的可视化能力，一些编程经验以及意愿和耐心去遵循一些非平凡的例子，没有比看袋鼠跳跃更能了解袋鼠的方式了。我们学习主流的技术，但是也不回避一些奇怪的技术，它们看起来似乎只具有理论上的有趣，但是它们经常提供新的见解，读者可能会在某些地方用得上。
```

> 1.2 The Approach Used (使用的方法)

```
This book addresses the reader at least three different levels. The interested noncomputer scientist can read the book as “the story of grammars and parsing”; he or she can skip the detailed explanations of the algorithms: each algorithm is first explained in general terms. The computer scientist will find much technical detail on a wide array of algorithms. To the expert we offer a systematic bibliography of over 1700 entries. The printed book holds only those entries referenced in the book itself; the full list is available on the web site of this book. All entries in the printed book and about two-thirds of the entries in the web site list come with an annotation; this annotation, or summary, is unrelated to the abstract in the referred article, but rather provides a short explanation of the contents and enough material for the reader to decide if the referred article is worth reading.

本书至少在三个不同的层次上对读者进行了说明。感兴趣的非计算机科学家可以把这本书看作“文法和解析的故事”。他或她可以跳过算法的详细解释：每种算法首先用一般术语解释。计算机科学家将在各种算法中找到许多技术细节。对于专家，我们提供了超过1700项的系统参考书目。印刷书籍只包含书本身引用的条目，完整列表可以在本书的网站上找到。印刷书籍中的所有条目和网站列表中约三分之二的条目都附带注释，这个注释或摘要与被引用文章中的摘要无关，而是提供了内容的简短说明，并提供了足够的材料供读者决定是否值得阅读。
```

```
No ready-to-run algorithms are given, except for the general context-free parser of Section 17.3. The formulation of a parsing algorithm with sufficient precision to enable a programmer to implement and run it without problems requires a considerable support mechanism that would be out of place in this book and in our experience does little to increase one’s understanding of the process involved. The popular methods are given in algorithmic form in most books on compiler construction. The less widely used methods are almost always described in detail in the original publication, for which see Chapter 18.

除了第17.3节的一般上下文无关语法分析器之外，没有提供任何现成的算法。具有足够精确性的解析算法的制定使得程序员能够毫无问题地实现和运行它，但是这需要相当大的支持机制，这在本书中时不合适的，而且根据我们的经验，这对增加一个人对所涉及的过程的理解几乎没有任何作用。在编译器构造的大多数书籍中，流行的方式都以算法的形式给出，原始出版物中几乎总是详细描述使用较少的方法，有关这些方法请参见第18章。
```

> 1.3 Outline of the Contents (大纲内容)

```
Since parsing is concerned with sentences and grammars and since grammars are themselves fairly complicated objects, ample attention is paid to them in Chapter 2. Chapter 3 discusses the principles behind parsing and gives a classification of parsing methods. In summary, parsing methods can be classified as top-down or bottom-up and as directional or non-directional; the directional methods can be further distinguished into deterministic and non-deterministic ones. This situation dictates the contents of the next few chapters.

由于语法分析涉及到句子和语法，而语法本身就是相当复杂的对象，所以在第二章中给予了充分的关注。第3章讨论了解析背后的原理，并给出了解析方法的分类。总之，解析方法可以分为自上而下或自下而上，并可分为定向和非定向的。定向方法可以进一步区分为确定性的和非确定性的。这种情况决定了接下来几章的内容。
```

```
In Chapter 4 we treat non-directional methods, including Unger and CYK. Chapter 5 forms an intermezzo with the treatment of finite-state automata, which are needed in the subsequent chapters. Chapters 6 through 10 are concerned with directional methods, as follows. Chapter 6 covers non-deterministic directional top-down parsers (recursive descent, Definite Clause Grammars), Chapter 7 non-deterministic directional bottom-up parsers (Earley). Deterministic methods are treated in Chapters 8 (top-down: LL in various forms) and 9 (bottom-up: LR methods). Chapter 10 covers non-canonical parsers, parsers that determine the nodes of a parse tree in a not strictly top-down or bottom-up order (for example left-corner). Non-deterministic versions of the above deterministic methods (for example the GLR parser) are described in Chapter 11.

在第4章中，我们讨论了非定向方法，包括Unger和CYK。第5章是对有限状态自动机的处理，这是后续章节所需要的。第6章到第10章涉及定向方法，如下所示。第6章讨论了非确定性定向自顶向下解析器（递归下降、确定子句文法），第7章介绍非确定性定向自下而上解析器(Earley)。确定性方法在第8章（自上而下：各种形式的LL）和第9章（自下而上：LR方法）中进行处理。第10章介绍了非规范解析器，这些解析器以非严格的自上而下或自下而上的顺序来确定解析树的节点。上述确定性方法（例如GLR解析器）的非确定性版本在第11章中描述。
```

```
The next four chapters are concerned with material that does not fit the above framework. Chapter 12 shows a number of recent techniques, both deterministic and non-deterministic, for parsing substrings of complete sentences in a language. Another recent development, in which parsing is viewed as intersecting a context-free grammar with a finite-state automaton is covered in Chapter 13. A few of the numerous parallel parsing algorithms are explained in Chapter 14, and a few of the numerous proposals for non-Chomsky language formalisms are explained in Chapter 15, with their parsers. That completes the parsing methods per se.

接下来的四章是关于不符合上述框架的材料。第12章展示了一些最近用于解析语言中完整句子的子串的确定性和非确定性技术。第13章介绍了另一个最近的发展，其中解析被看作是将上下文无关文法与有限状态自动机相交的例子。在第14章中介绍了许多并行解析算法，并在第15章中介绍了非Chomskey语言形式的许多提议，这就完成了解析方法本身。
```

```
Error handling for a selected number of methods is treated in Chapter 16, and Chapter 17 discusses practical parser writing and use.

在第16章中介绍了对一些选定数量的方法的错误处理，第17章讨论了实际解析器的编写和使用。
```

> 1.4 The Annotated Bibliography (带注释的数目)

```
The annotated bibliography is presented in Chapter 18 both in the printed book and, in a much larger version, on the web site of this book. It is an easily accessible and essential supplement of the main body of the book. Rather than listing all publications in author-alphabetic order, the bibliography is divided into a number of named sections, each concerned with a particular aspect of parsing; there are 25 of them in the printed book and 30 in the web bibliography. Within the sections, the publications are listed chronologically. An author index at the end of the book replaces the usual alphabetic list of publications. A numerical reference placed in brackets is used in the text to refer to a publication. For example, the annotated reference to Earley’s publication of the Earley parser is indicated in the text by [14] and can be found on page 578, in the entry marked 14.

注释书目在第18章中出现在印刷书籍和本书的网站上，它是本书主体的一个易读和必要的补充。参考数目不是按照作者-字母顺序列出所有，而是分成若干命名部分，每个部分都与解析的一个特定方面有关。印刷书籍中有25个，为网络参考书目中有30个。在这些部分中，出版物按照时间顺序排列。书末的作者索引取代了通常按字母顺序排列的出版物列表。括号内的数字引用指代出版物。例如在Earley的Earley解析器出版物中的注释引用在[14]文本中指出，并可以在第578页标记为14的条目中找到。
```
