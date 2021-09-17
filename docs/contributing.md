# Contributing guidelines

When contributing to the JS module it is important to follow the guidelines to make sure your pull request makes it easy for you
and the module maintainers and reviewers.

When you don't follow these guidelines, your pull request might be closed or will take a long time to be reviewed/merged.

## Pull request
### Title
A *clear and concise* pull request title is important for identifying your pull request.

The title should generally follow the same rules as the [commit names](#commit-names).
But the prefix that is added to the commit name, is not needed in the pull request name.

If a pull request only has a single commit in it, it is allowed to copy that commit name
into the title, **as long as the commit name follows the [commit guidelines](#commits)**.

### Body
The body of a pull request should give the reviewer(s) and maintainer(s) all information about the pull request they need.
This could for example be an issue related that is fixed by that pull request, or an explanation to why the change was done or
an explanation for the code that was added.

The most important things to answer in the pull request body is *why* the pull request was opened and *how* the desired
change was done.

## Commits
### General
The right amount of commits is always very dependent on what is being done, how it is being done and personal taste.
Therefore it is not easy to write a clear guideline on how many commits should be made and how.

A good rule of thumb for creating commits is that the changes in a commit should all depend on each other and be related
to each other.

If you create too many commits, it is also possible to *squash* a group of commits into a single commit before creating a pull request.

### Commit names
To make changes to the codebase easily trackable for the future, this repository follows a very simple schema for naming commits.
The schema is as follows:
```
{CATEGORY}: {TYPE} {DESCRIPTION}
```

The categories that should be used are `docs`, `server`, `client` and `shared`.

These should be used depending on which part of the module was changed.

If a single commit applies to multiple categories, then they can be chained with a `/`.

The types that should be used are not clearly defined, but they are for example `fix`, `add`, `remove` and so on.
Whether to use these in the present tense or in the past tense is up to you, and is generally unimportant.

A few examples for commit names that follow this schema are the following:
```
docs: Fix typo in contributing article
```
The category here would be `docs` and the type is `fix`.

```
client/server: Remove unused macro
```
The categories here would be `client` and `server`, and the type is `remove`.

**If you are unsure whether your commit name is following this schema, ask a module maintainer to review the name for you.**

### Additional commit details
It is allowed to add additional comments and informations to individual commits, but it is not required.
If you think an explanation or further clarification should be added, then don't hesitate to do so.

> Make sure to write *meaningful* and *clear* commit descriptions!

## Code
### Comments
When adding code to the codebase it is important to document your code in a way that a future developer
can look at your work and understand how it works, and how and why things are done how they are.

The amount and depth of comments is very *subjective* and there is no clear guideline for it.
But it is important to make sure to not add any *useless* comments that explain things that are very
clear to the reader. <br>
Instead, explain things that are harder to understand when first looking at it.

Small additions/fixes like the addition of a new API method does not need any comments in most situations.
They are only important when making bigger changes to the codebase.

It is up to the reviewing module maintainer(s) to decide whether or not the documentation is sufficient,
and if they think that the documentation for the code is lacking, they might ask you to add more documentation
to your code.

### Style
~~This repository includes `.editorconfig` and `ClangFormat` code style definitions to make sure the style is consistent across the whole codebase.~~

~~Please make sure to configure your code editor to conform to these style definitions, it is also recommended to enable `Format on save`
to make sure you don't forget to format the files using the provided style.~~

**This section is currently under construction.**

### ToDos
It is allowed to add `todo` comments to your code, for e.g. later optimizations that aren't needed for the initial implementation
or *small non-critical* bugs that need to be fixed.

These do not need to be pre/postfixed with the name of the developer adding that todo.
So an example todo comment would look like this:
```cpp
// todo: Use std::set instead of std::list
```

### Deprecations
When changing a function name for example it is important to **not** remove the old function, but instead add a deprecation warning
so we can provide backwards compatibility.

A deprecation warning can just be added by using `Log::Warning` to the end of the old function body.
It should also have a comment with the *date and current alt:V release version* when the deprecation was added.

After a reasonable amount of times deprecated functions are removed, usually after a few release updates.

An example for a deprecation can be found here:
```cpp
// Deprecation added: 09/17/2021 (version 5.3)
Log::Warning << "alt.oldFunction is deprecated. Consider using alt.newFunction instead." << Log::Endl;
```

## Contact

If you have any questions about making a pull request, don't hesitate to contact a module maintainer, 
you can usually find them by asking on the [official Discord server](https://discord.altv.mp).
