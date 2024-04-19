project(
  'ftn2xml',
  'cpp',
  license : 'GPL-3.0-or-later',
  default_options : ['buildtype=release', 'prefix=/usr']
)

prefix = get_option('prefix')
project_datadir = get_option('datadir') / meson.project_name()
project_docdir = get_option('datadir') / 'doc' / meson.project_name()

doc_readme = 'Readme.md'
doc_syntax = 'Fountain_Syntax.md'

pdf_export = get_option('pdf_export')
if not pdf_export.disabled()
  podofo = dependency('libpodofo', required: pdf_export)
else
  podofo = disabler()
endif

conf_data = configuration_data()
conf_data.set('version', meson.project_version())

conf_data.set('prefix', prefix)
conf_data.set('project_datadir', project_datadir)
conf_data.set10('pdf_export', podofo.found())

config_h = configure_file(
  input: 'config.h.in',
  output: 'config.h',
  configuration: conf_data,
)

executable(
  meson.project_name(),
  sources: [
    config_h,
    'source/auxiliary.cxx',
    'source/fountain.cxx',
    'source/ftn2xml.cxx',
  ],
  dependencies: [podofo],
  name_prefix: '',
  install: true,
  install_dir: get_option('bindir'),
)

if meson.version().version_compare('>=0.61.0')
  install_symlink(
    'ftn2fdx',
    install_dir: get_option('bindir'),
    pointing_to: meson.project_name(),
  )

  install_symlink(
    'ftn2html',
    install_dir: get_option('bindir'),
    pointing_to: meson.project_name(),
  )

  if (podofo.found())
    install_symlink(
      'ftn2pdf',
      install_dir: get_option('bindir'),
      pointing_to: meson.project_name(),
    )
  endif
endif

install_data(
  sources: [
    'data/fountain-html.css',
    'data/fountain-xml.css',
    'data/screenplain.css',
    'data/textplay.css',
  ],
  install_dir: project_datadir,
)

install_data(
  sources: [
    doc_syntax,
    doc_readme,
  ],
  install_dir: project_docdir,
)
