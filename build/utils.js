import path from 'path';
import config from '../config';
import MiniCssExtractPlugin from 'mini-css-extract-plugin';

export function assetsPath(_path) {
  const assetsSubDirectory =
    process.env.NODE_ENV === 'production'
      ? config.build.assetsSubDirectory
      : config.dev.assetsSubDirectory;
  return path.posix.join(assetsSubDirectory, _path);
}

export function cssLoaders(options = {}) {
  function generateLoaders(loaders) {
    const sourceLoader = loaders
      .map((loader) => {
        const [name, options] = loader.split('?');
        return {
          loader: `${name}-loader`,
          options: options ? { sourceMap: options.sourceMap, ...options } : { sourceMap: options.sourceMap },
        };
      });

    if (options.extract) {
      return [MiniCssExtractPlugin.loader, ...sourceLoader];
    } else {
      return ['vue-style-loader', ...sourceLoader];
    }
  }

  return {
    css: generateLoaders(['css']),
    postcss: generateLoaders(['postcss']),
    less: generateLoaders(['less']),
    sass: generateLoaders(['sass?indentedSyntax=true']),
    scss: generateLoaders(['sass']),
    stylus: generateLoaders(['stylus']),
    styl: generateLoaders(['stylus']),
  };
}

export function styleLoaders(options) {
  const loaders = cssLoaders(options);
  return Object.entries(loaders).map(([extension, loader]) => ({
    test: new RegExp(`\\.${extension}$`),
    use: loader,
  }));
}
