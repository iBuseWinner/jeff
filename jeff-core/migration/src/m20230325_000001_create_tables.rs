use sea_orm_migration::prelude::*;

#[derive(DeriveMigrationName)]
pub struct Migration;

#[async_trait::async_trait]
impl MigrationTrait for Migration {
  async fn up(&self, manager: &SchemaManager) -> Result<(), DbErr> {
    manager
      .create_table(
        Table::create()
          .table(User::Table)
          .if_not_exists()
          .col(ColumnDef::new(User::Id).big_integer().not_null().auto_increment().primary_key())
          .col(ColumnDef::new(User::Login).string().not_null().unique_key())
          .col(ColumnDef::new(User::PHash).string().not_null())
          .col(ColumnDef::new(User::Settings).string().not_null())
          .to_owned()
      ).await?;
    manager
      .create_table(
        Table::create()
          .table(Phrase::Table)
          .if_not_exists()
          .col(ColumnDef::new(Phrase::Id).big_integer().not_null().auto_increment().primary_key())
          .col(ColumnDef::new(Phrase::Text).string().not_null())
          .col(ColumnDef::new(Phrase::Links).string())
          .to_owned()
      ).await?;
    manager
      .create_table(
        Table::create()
          .table(Source::Table)
          .if_not_exists()
          .col(ColumnDef::new(Source::Id).big_integer().not_null().auto_increment().primary_key())
          .col(ColumnDef::new(Source::Users).string().not_null())
          .col(ColumnDef::new(Source::Name).string().not_null())
          .to_owned()
       ).await
  }

  async fn down(&self, manager: &SchemaManager) -> Result<(), DbErr> {
    manager.drop_table(Table::drop().table(User::Table).to_owned()).await?;
    manager.drop_table(Table::drop().table(Phrase::Table).to_owned()).await?;
    manager.drop_table(Table::drop().table(Source::Table).to_owned()).await
  }
}

/// Learn more at https://docs.rs/sea-query#iden
#[derive(Iden)]
enum User {
  Table,
  Id,
  Login,
  PHash,
  Settings,
}

#[derive(Iden)]
enum Phrase {
  Table,
  Id,
  Text,
  Links,
}

#[derive(Iden)]
enum Source {
  Table,
  Id,
  Users,
  Name
}
