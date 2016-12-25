LibSass currently expects all input to be utf8 encoded (and outputs only utf8), if you actually have any unicode characters at all. We do not support conversion between encodings, even if you declare it with a `@charset` rule. The text below was originally posted as an [issue](https://github.com/sass/libsass/issues/381) on the LibSass tracker.

### [Declaring character encodings in CSS](http://www.w3.org/International/questions/qa-css-charset.en)

This [explains](http://www.w3.org/International/questions/qa-css-charset.en) how the character encoding of a css file is determined. Since we are only dealing with local files, we never have a HTTP header. So the precedence should be 'charset' rule, byte-order mark (BOM) or auto-detection (finally falling back to system default/UTF-8). This may not sound too hard to implement, but what about import rules? The CSS specs do not forbid the mixing of different encodings! I solved that by converting all files to UTF-8 internally. On writing there is an option to tell the tool what encoding it should be (UTF-8 by default). One can also define if it should write a BOM or not and if it should add the charset declaration.

Since my tool is written in perl, I have a lot of utilities at hand to deal with different unicode charsets. I'm pretty sure that most OSS uses [libiconv](https://www.gnu.org/software/libiconv/) to convert between different encodings. But I have now idea how easy/hard this would be to integrate platform independent (it seems doable).

### Current status on LibSass unicode support

Currently LibSass seems to handle the common UTF-8 case pretty well. I believe it should correctly support all ASCII compatible encodings (like UTF-8 or Latin-1). If all includes use the same encoding, the output should be correct (in the same encoding). It should also handle unicode chars in [selectors, variable names and other identifiers](https://github.com/hcatlin/libsass/issues/244#issuecomment-34681227). This is true for all ASCII compatible encodings. So the main incompatible encodings (I'm aware of) are UTF-16/UTF-32 (which could be converted to UTF-8 with libiconv).

### Current encoding auto detection

LibSass currently reads all kind of BOMs and will error out if it finds something it doesn't know how to handle! It seems that it throws away the optional UTF-8 BOM (if any is found). IMO it would be nice if users could configure that (also if a charset rule should be added to the output).

### What is currently not supported

- Using non ASCII compatible encodings (like UTF-16)
- Using non ASCII characters in different encodings in different includes

### What is missing to support the above cases

- A way to convert between encodings (like libiconv)
- Sniffing the charset inside the file (source is available)
- Handling the conversion on import (and export)
- Optional: Make output encoding configurable
- Optional: Add optional/mandatory BOM (configurable)

### Low priority feature

I guess the current implementation should handle more than 99% of all real world use cases.
A) Unicode characters are still seldomly seen (as they can be written escaped)
B) It will still work if it's UTF-8 or in any of the most common known western ISO codepages.
Although I'm not sure how this applies to asian and other "exotic" codepages!

I guess the biggest Problem is to have libiconv (or some other) library as a dependency. Since it contains a lot of rules for the conversions, I see it as the only way to handle this correctly. Once that is sorted out it should be pretty much straight forward to implement the missing pieces (in parser.cpp - Parser::parse should return encoding and add Parser::sniff_charset, then convert the source byte stream to UTF-8).

I hope the statements above all hold true. Unicode is really not the easiest topic to wrap your head around. But since I did all the above recently in Perl, I wanted to document it here. Feel free to extend or criticize.
